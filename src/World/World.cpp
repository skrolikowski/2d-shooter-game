#include <cmath>
#include <numbers>

#include "Boss.h"
#include "Event/EventBus.h"
#include "Player.h"
#include "Util/Utility.h"
#include "World.h"

World::World()
{
    // Create Player..
    player_ = std::make_unique<Player>();
    player_->SetPosition(Vector2<float>{ SCREEN_WIDTH / 2.F, SCREEN_HEIGHT - 80.F });
    player_->SetWorld(this);

    // Create Invader Manager..
    formation_ = std::make_unique<Formation>();
    formation_->SetWorld(this);
}

/**
 * Notes:
 * Now that player_ is a unique_ptr<Player>, destroying World means
 * deleting a Player, and the header only has the forward declaration.
 *
 * Rule of thumb: a unique_ptr<T> member of a forward-declared T means
 * the destructor body goes in the .cpp. It's a common C++ idiom, often
 * called "pimpl-style" destruction.
 */
World::~World() = default;

bool World::SpawnBunker(Vector2<float> pos)
{
    Bunker *b = bunkers_.Spawn(pos);
    return b != nullptr;
}

void World::GivePlayerPowerUp(PowerUpType powerUpType)
{
    if (player_ != nullptr)
        player_->GainPowerUp(powerUpType);
}

void World::RemovePlayerPowerUp(PowerUpType powerUpType)
{
    if (player_ != nullptr)
        player_->LosePowerUp(powerUpType);
}

bool World::SpawnPowerUp(Vector2<float> pos, PowerUpType powerUpType)
{
    PowerUp *p = powerUps_.Spawn(pos);
    if (p != nullptr)
    {
        p->SetType(powerUpType);
        return true;
    }
    return false;
}

bool World::SpawnEnemyBullet(Vector2<float> pos)
{
    Bullet *b = enemyBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetSpeed(300);
        b->SetDir(Vector2<float>{ 0.F, 1.F });
        return true;
    }
    return false;
}

bool World::SpawnBossBullet(Vector2<float> pos)
{
    Bullet *b = enemyBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetTravelTween({ .from = 50.F, .to = 400.F, .duration = 0.5F, .ease = Easing::InQuad });
        b->SetDir(Vector2<float>{ 0.F, 1.F });
        return true;
    }
    return false;
}

bool World::SpawnPlayerBullet(Vector2<float> pos)
{
    // Check PowerUps!!
    bool hasRapidFire = player_->HasPowerUp(PowerUpType::RapidFire);
    bool hasSpreadShot = player_->HasPowerUp(PowerUpType::SpreadShot);

    // Set hard-cap bullet spawn limit..
    std::size_t limit = PLAYER_BULLET_LIMIT;
    if (hasRapidFire)
        limit *= POWERUP_RAPIDFIRE_MULTIPLIER;
    if (hasSpreadShot)
        limit *= POWERUP_SPREAD_MULTIPLIER;
    limit = std::max<std::size_t>(PLAYER_BULLET_LIMIT, limit);

    // Hard-cap limit.. (based on PowerUp status)
    if (playerBullets_.CountAlive() >= limit)
        return false;

    auto spawnBullet = [&](Vector2<float> dir)
    {
        Bullet *b = playerBullets_.Spawn(pos);
        if (b != nullptr)
        {
            b->SetSpeed(500);
            b->SetDir(dir);
        }
    };

    if (hasSpreadShot)
    {
        // Spawn spread shot..
        double degrees = 60.0;
        double radians = degrees * M_PI / 180.0;
        float spreadX = static_cast<float>(std::cos(radians));

        spawnBullet(Vector2<float>{ 0.F, -1.F });
        spawnBullet(Vector2<float>{ -spreadX, -1.F });
        spawnBullet(Vector2<float>{ spreadX, -1.F });
    }
    else
    {
        // Spawn single bullet..
        spawnBullet(Vector2<float>{ 0.F, -1.F });
    }
    return true;
}

void World::DamagePlayer(int amount)
{
    player_->Damage(amount);

    if (player_->IsDead())
        SpawnExplosion(player_->GetCenter());
}

bool World::PlayerHasPowerUp(PowerUpType powerUpType) const
{
    return player_->HasPowerUp(powerUpType);
}

void World::SpawnBoss(const json &data)
{
    boss_ =
        std::make_unique<Boss>(Vector2{ (SCREEN_WIDTH / 2.F) - 64.F, SCREEN_HEIGHT * 0.25F }, data);
    boss_->SetWorld(this);
}

void World::SpawnExplosion(Vector2<float> pos)
{
    std::mt19937 rng_{ std::random_device{}() };
    std::uniform_real_distribution<float> angle(0.F, 2.F * static_cast<float>(std::numbers::pi));
    std::uniform_real_distribution<float> health(1.F, 2.5F);
    std::uniform_real_distribution<float> speed(50.F, 150.F);
    std::uniform_int_distribution<int> size(2, 8);
    std::uniform_real_distribution<float> tint(0.F, 1.F);

    for (std::size_t i = 0; i < EXPLOSION_PARTICLES_PER; ++i)
    {
        const float randParticleAngle = angle(rng_);
        const float randParticleHealth = health(rng_);
        const float randParticleSpeed = speed(rng_);
        const int randParticleSize = size(rng_);
        const float randParticleTint = tint(rng_);

        Particle *p = particles_.Spawn(pos);
        if (p != nullptr)
        {
            p->vel = Vector2<float>{
                std::sin(randParticleAngle) * randParticleSpeed,
                std::cos(randParticleAngle) * randParticleSpeed,
            };
            // Random color between range..
            p->color =
                Lerp(SDL_FColor{ 1, 0.9F, 0.2F, 1 }, SDL_FColor{ 1, 0.3F, 0, 1 }, randParticleTint);
            p->size.x = randParticleSize;
            p->size.y = randParticleSize;
            p->SetHealth(static_cast<int>(randParticleHealth));
        }
    }
}

void World::SetPlayerPosition(Vector2<float> pos)
{
    if (player_ != nullptr)
        player_->SetPosition(pos);
}

void World::Clear()
{
    playerBullets_.Clear();
    enemyBullets_.Clear();
    bunkers_.Clear();
    particles_.Clear();
    powerUps_.Clear();

    if (formation_ != nullptr)
        formation_->Clear();

    if (boss_ != nullptr)
        boss_.reset();
}

void World::HandleInput()
{
    if (player_ != nullptr)
        player_->HandleInput();
}

void World::Update(float dt)
{
    // --
    updateFormation(dt);
    updateBoss(dt);
    updatePlayer(dt);

    // --
    // Update pools..
    playerBullets_.ForEach([&](Bullet &bullet) { bullet.UpdateAndSweep(dt); });
    enemyBullets_.ForEach([&](Bullet &bullet) { bullet.UpdateAndSweep(dt); });
    powerUps_.ForEach([&](PowerUp &powerUp) { powerUp.Update(dt); });
    particles_.ForEach([&](Particle &p) { p.Update(dt); });

    // ----
    // Collision Detection
    // ----

    // Check Player / Formation..
    if (player_ && formation_)
    {
        Invader *invader = formation_->TryHit(player_->GetRect());
        if (invader != nullptr)
        {
            PlayerHit ev;
            ev.isInvaderFire = false;
            EventBus::GetInstance().Push(ev);
        }
    }

    // if (player_ && boss_)
    // {
    //     SDL_FRect playerRect = player_->GetRect();
    //     SDL_FRect bossRect = boss_->GetRect();
    //     if (SDL_HasRectIntersectionFloat(&playerRect, &bossRect))
    //     {
    //         PlayerHit ev;
    //         ev.isInvaderFire = false;
    //         EventBus::GetInstance().Push(ev);
    //     }
    // }

    // Check Player Bullet / Formation..
    if (formation_)
    {
        playerBullets_.ForEach(
            [&](Bullet &bullet)
            {
                if (bullet.IsDead())
                    return; // bullet may have died already..

                if (Invader *invader = formation_->TryHit(bullet.GetSweep()))
                {
                    invader->Damage(1);
                    bullet.Damage(1); // Warning: bullets may die

                    if (invader->IsDead())
                    {
                        InvaderKilled ev;
                        ev.pos = invader->GetCenter();
                        ev.type = invader->GetType();
                        EventBus::GetInstance().Push(ev);

                        if (formation_->IsDead())
                        {
                            EventBus::GetInstance().Push(WaveCleared{});
                        }
                    }
                }
            });

        // Formation / Bunker Collision Check..
        bunkers_.ForEach(
            [&](Bunker &bunker)
            {
                formation_->ForEachInvader(
                    [&](Invader &invader) { bunker.Carve(invader.GetRect()); });
            });
    }

    if (player_)
    {
        // Check Invader Bullet / Player..
        enemyBullets_.ForEach(
            [&](Bullet &bullet)
            {
                if (bullet.IsDead())
                    return; // may have died already..

                SDL_FRect playerRect = player_->GetRect();
                if (SDL_HasRectIntersectionFloat(&playerRect, &bullet.GetSweep()))
                {
                    bullet.Damage(1); // Warning: bullets may die

                    PlayerHit ev{};
                    ev.isInvaderFire = true;
                    EventBus::GetInstance().Push(ev);
                }
            });

        powerUps_.ForEach(
            [&](PowerUp &powerUp)
            {
                if (powerUp.IsDead())
                    return; // may have died already..

                SDL_FRect playerRect = player_->GetRect();
                SDL_FRect powerUpRect = powerUp.GetRect();
                if (SDL_HasRectIntersectionFloat(&playerRect, &powerUpRect))
                {
                    powerUp.Damage(1);

                    EventBus::GetInstance().Push(PowerUpGrabbed{ powerUp.GetType() });
                }
            });
    }

    if (boss_ != nullptr)
    {
        playerBullets_.ForEach(
            [&](Bullet &bullet)
            {
                if (bullet.IsDead())
                    return; // may have died already..

                SDL_FRect bossRect = boss_->GetRect();
                if (SDL_HasRectIntersectionFloat(&bossRect, &bullet.GetSweep()))
                {
                    boss_->Damage(1);
                    bullet.Damage(1); // Warning: bullets may die

                    if (boss_->IsDead())
                    {
                        BossKilled ev;
                        ev.pos = boss_->GetCenter();
                        ev.points = BOSS_POINT_AMOUNT;
                        EventBus::GetInstance().Push(ev);

                        EventBus::GetInstance().Push(WaveCleared{});
                    }
                }
            });
    }

    // Check Bullet / Bunker Collision..
    bunkers_.ForEach(
        [&](Bunker &bunker)
        {
            playerBullets_.ForEach(
                [&](Bullet &bullet)
                {
                    if (bullet.IsDead())
                        return; // bullet may have died already..

                    std::optional<SDL_FPoint> collisionPoint = bunker.FindImpact(bullet.GetSweep());
                    if (collisionPoint.has_value())
                    {
                        bullet.Damage(1);
                        bunker.Erode(collisionPoint.value());
                    }
                });

            enemyBullets_.ForEach(
                [&](Bullet &bullet)
                {
                    if (bullet.IsDead())
                        return; // bullet may have died already..

                    std::optional<SDL_FPoint> collisionPoint =
                        bunker.FindImpact(bullet.GetSweep(), false);
                    if (collisionPoint.has_value())
                    {
                        bullet.Damage(1);
                        bunker.Erode(collisionPoint.value());
                    }
                });
        });
}

void World::Draw() const
{
    if (player_ != nullptr)
        player_->Draw();

    if (formation_ != nullptr)
        formation_->Draw();

    if (boss_ != nullptr)
        boss_->Draw();

    // Draw pools..
    playerBullets_.ForEach([&](const Bullet &b) { b.Draw(); });
    enemyBullets_.ForEach([&](const Bullet &b) { b.Draw(); });
    bunkers_.ForEach([&](const Bunker &b) { b.Draw(); });
    particles_.ForEach([](const Particle &p) { p.Draw(); });
    powerUps_.ForEach([](const PowerUp &p) { p.Draw(); });
}

void World::updateFormation(float dt)
{
    if (formation_ != nullptr)
        formation_->Update(dt);
}

void World::updateBoss(float dt)
{
    if (boss_ != nullptr)
        boss_->Update(dt);
}

void World::updatePlayer(float dt)
{
    if (player_ != nullptr)
        player_->Update(dt);
}
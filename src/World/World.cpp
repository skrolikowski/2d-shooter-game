#include <cmath>
#include <numbers>

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

bool World::SpawnInvaderBullet(Vector2<float> pos)
{
    Bullet *b = invaderBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetDir(Vector2<float>{ 0.F, 1.F });
        return true;
    }
    return false;
}

bool World::SpawnPlayerBullet(Vector2<float> pos)
{
    Bullet *b = playerBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetDir(Vector2<float>{ 0.F, -1.F });
        return true;
    }
    return false;
}

void World::DamagePlayer(int amount)
{
    player_->Damage(amount);

    if (player_->IsDead())
        SpawnExplosion(player_->GetCenter());
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
    invaderBullets_.Clear();
    bunkers_.Clear();
    particles_.Clear();

    if (formation_ != nullptr)
        formation_->Clear();
}

void World::HandleInput()
{
    if (player_ != nullptr)
        player_->HandleInput();
}

void World::Update(float dt)
{
    // --
    // Update Formation..
    if (formation_ != nullptr)
        formation_->Update(dt);

    // --
    // Update Player..
    if (player_ != nullptr)
        player_->Update(dt);

    // --
    // Update Player Bullets..
    playerBullets_.ForEach(
        [&](Bullet &bullet)
        {
            /**
             * Dev notes:
             * Create a SDL_FRect of the full Bullet path to combat Collision Tunneling
             */
            SDL_FRect before;
            SDL_FRect after;
            SDL_FRect sweep;
            before = bullet.GetRect();
            bullet.Update(dt);
            after = bullet.GetRect();
            SDL_GetRectUnionFloat(&before, &after, &sweep);
            bullet.SetSweep(sweep);
        });

    // --
    // Update Invader Bullets..
    invaderBullets_.ForEach(
        [&](Bullet &bullet)
        {
            /**
             * Dev notes:
             * Create a SDL_FRect of the full Bullet path to combat Collision Tunneling
             */
            SDL_FRect before;
            SDL_FRect after;
            SDL_FRect sweep;
            before = bullet.GetRect();
            bullet.Update(dt);
            after = bullet.GetRect();
            SDL_GetRectUnionFloat(&before, &after, &sweep);
            bullet.SetSweep(sweep);
        });

    // ----
    // Collision Detection
    // ----

    // Check Player / Formation..
    if (player_ && formation_)
    {
        if (formation_->TryHit(player_->GetRect()) != nullptr)
            EventBus::GetInstance().Push(PlayerHit{});
    }

    // Check Player Bullet / Formation..
    if (formation_)
    {
        playerBullets_.ForEach(
            [&](Bullet &bullet)
            {
                if (bullet.IsDead())
                    return; // bullet may have died already..

                if (Entity *invader = formation_->TryHit(bullet.GetSweep()))
                {
                    invader->Damage(1);
                    bullet.Damage(1); // Warning: bullets may die

                    if (invader->IsDead())
                    {
                        EventBus::GetInstance().Push(
                            InvaderKilled{ invader->GetCenter(), invader->GetType() });

                        if (formation_->IsDead())
                        {
                            EventBus::GetInstance().Push(WaveCleared{});
                        }
                    }
                }
            });
    }

    // Check Invader Bullet / Player..
    if (player_)
    {
        invaderBullets_.ForEach(
            [&](Bullet &bullet)
            {
                if (bullet.IsDead())
                    return; // bullet may have died already..

                SDL_FRect playerRect = player_->GetRect();
                if (SDL_HasRectIntersectionFloat(&playerRect, &bullet.GetSweep()))
                {
                    bullet.Damage(1); // Warning: bullets may die

                    EventBus::GetInstance().Push(PlayerHit{});
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

            invaderBullets_.ForEach(
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

    particles_.ForEach([&](Particle &p) { p.Update(dt); });
}

void World::Draw() const
{
    if (player_ != nullptr && !player_->IsDead())
        player_->Draw();

    playerBullets_.ForEach([&](const Bullet &b) { b.Draw(); });
    invaderBullets_.ForEach([&](const Bullet &b) { b.Draw(); });
    bunkers_.ForEach([&](const Bunker &b) { b.Draw(); });
    particles_.ForEach([](const Particle &p) { p.Draw(); });

    if (formation_ != nullptr)
        formation_->Draw();
}
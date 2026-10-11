#include "World.h"
#include "Player.h"
#include "Util/TextureManager.h"

World::World()
{
    // Create Player..
    player_ = std::make_unique<Player>();
    player_->SetPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 80.f });
    player_->SetWorld(this);
    player_->onShoot = [this]{ if (onPlayerShoot) onPlayerShoot(); };
    // ..
    std::shared_ptr<Texture> playerTexture = TextureManager::Get("res/player_red.png");
    player_->SetTexture(playerTexture);
    
    // Create Invader Manager..
    formation_ = std::make_unique<Formation>();
    formation_->SetWorld(this);
    formation_->onShoot = [this]{ if (onInvaderShoot) onInvaderShoot(); };
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
    Bunker* b = bunkers_.Spawn(pos);
    return b != nullptr;
}

bool World::SpawnInvaderBullet(Vector2<float> pos)
{
    Bullet* b = invaderBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetDir(Vector2<float>{ 0.f, 1.f });
        return true;
    }
    return false;
}

bool World::SpawnPlayerBullet(Vector2<float> pos)
{
    Bullet* b = playerBullets_.Spawn(pos);
    if (b != nullptr)
    {
        b->SetDir(Vector2<float>{ 0.f, -1.f });
        return true;
    }
    return false;
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
    playerBullets_.ForEach([&](Bullet& bullet)
    {
        /**
         * Dev notes:
         * Create a SDL_FRect of the full Bullet path to combat Collision Tunneling
         */
        SDL_FRect before, after, sweep;
        before = bullet.GetRect();
        bullet.Update(dt);
        after = bullet.GetRect();
        SDL_GetRectUnionFloat(&before, &after, &sweep);
        bullet.SetSweep(sweep);
    });

    // --
    // Update Invader Bullets..
    invaderBullets_.ForEach([&](Bullet& bullet)
    {
        /**
         * Dev notes:
         * Create a SDL_FRect of the full Bullet path to combat Collision Tunneling
         */
        SDL_FRect before, after, sweep;
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
        if (formation_->TryHit(player_->GetRect()))
        {
            if (onPlayerHit)
                onPlayerHit();
        }
    }

    // Check Player Bullet / Formation..
    if (formation_)
    {
        playerBullets_.ForEach([&](Bullet& bullet)
        {
            if (bullet.IsDead()) return;  // bullet may have died already..
            
            if (formation_->TryHit(bullet.GetSweep()))
            {
                bullet.Damage(1);  // Warning: bullets may die

                if (onInvaderKilled)
                {
                    onInvaderKilled();
                    
                    if (formation_->IsDead())
                    {
                        onFormationKilled();
                    }
                }
            }
        });
    }

    // Check Invader Bullet / Player..
    if (player_)
    {
        invaderBullets_.ForEach([&](Bullet& bullet)
        {
            if (bullet.IsDead()) return;  // bullet may have died already..
            
            SDL_FRect playerRect = player_->GetRect();
            if (SDL_HasRectIntersectionFloat(&playerRect, &bullet.GetSweep()))
            {
                bullet.Damage(1); // Warning: bullets may die

                if (onPlayerHit)
                    onPlayerHit();
            }
        });
    }

    // Check Bullet / Bunker Collision..
    bunkers_.ForEach([&](Bunker& bunker)
    {
        playerBullets_.ForEach([&](Bullet& bullet)
        {
            if (bullet.IsDead()) return;  // bullet may have died already..
           
            std::optional<SDL_FPoint> collisionPoint = bunker.FindImpact(bullet.GetSweep());
            if (collisionPoint.has_value())
            {
                bullet.Damage(1);
                bunker.Erode(collisionPoint.value());
            }
        });

        invaderBullets_.ForEach([&](Bullet& bullet)
        {
            if (bullet.IsDead()) return;  // bullet may have died already..

            std::optional<SDL_FPoint> collisionPoint = bunker.FindImpact(bullet.GetSweep(), false);
            if (collisionPoint.has_value())
            {
                bullet.Damage(1);
                bunker.Erode(collisionPoint.value());
            }
        });
    });
}

void World::Draw(SDL_Renderer* renderer) const
{
    if (player_ != nullptr)
        player_->Draw(renderer);

    playerBullets_.ForEach([&](const Bullet& b)
    {
        b.Draw(renderer);
    });

    invaderBullets_.ForEach([&](const Bullet& b)
    {
        b.Draw(renderer);
    });

    bunkers_.ForEach([&](const Bunker& b)
    {
        b.Draw(renderer);
    });

    if (formation_ != nullptr)
        formation_->Draw(renderer);
}
#include "World.h"
#include "Player.h"

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

void World::AddPlayer(std::unique_ptr<Player> player)
{
    player_ = std::move(player);
    player_->SetWorld(this);
    player_->onShoot = [this]{ if (onPlayerShoot) onPlayerShoot(); };
    // Add(std::move(player));
}

void World::AddFormation(std::unique_ptr<Formation> formation)
{
    formation->SetWorld(this);
    formation_ = std::move(formation);
}

bool World::SpawnBunker(Vector2<float> pos)
{
    Bunker* b = bunkers_.Spawn(pos);
    return b != nullptr;
}

bool World::SpawnPlayerBullet(Vector2<float> pos)
{
    Bullet* b = playerBullets_.Spawn(pos);
    return b != nullptr;
}

void World::SetPlayerPosition(Vector2<float> pos)
{
    if (player_ != nullptr)
        player_->SetPosition(pos);
}

void World::Clear()
{
    player_ = nullptr;
    playerBullets_.Clear();
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
    if (formation_ != nullptr)
    {
        // Update formation..
        formation_->Update(dt);

        // Check Player / Formation Collision..
        if (player_ != nullptr)
        {
            if (formation_->TryHit(player_->GetRect()))
            {
                if (onPlayerHit)
                    onPlayerHit();
            }
        }

        playerBullets_.ForEach([&](Bullet& bullet)
        {
            if (formation_->TryHit(bullet.GetRect()))
            {
                bullet.Damage(1);

                if (onEnemyKilled)
                    onEnemyKilled();
            }
        });
    }

    // Update Player..
    if (player_ != nullptr)
        player_->Update(dt);

    // Update Player Bullets..
    playerBullets_.ForEach([&](Bullet& bullet)
    {
        bullet.Update(dt);

        // Check Bullet / Bunker Collision..
        bunkers_.ForEach([&](Bunker& bunker)
        {
            if (bunker.TryHit(bullet.GetRect()))
            {
                bullet.Damage(1);
                bunker.Erode(bullet.GetRect());
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

    bunkers_.ForEach([&](const Bunker& b)
    {
        b.Draw(renderer);
    });

    if (formation_ != nullptr)
        formation_->Draw(renderer);
}
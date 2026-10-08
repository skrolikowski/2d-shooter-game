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

bool World::SpawnPlayerBullet(Vector2<float> v)
{
    Bullet* b = playerBullets_.Spawn(v);
    return b != nullptr;
}

void World::SetPlayerPosition(Vector2<float> v)
{
    if (player_ != nullptr)
        player_->SetPosition(v);
}

void World::Clear()
{
    player_ = nullptr;
    playerBullets_.Clear();

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

        playerBullets_.ForEach([&](Bullet& b)
        {
            if (formation_->TryHit(b.GetRect()))
            {
                b.Damage(1);

                if (onEnemyKilled)
                    onEnemyKilled();
            }
        });
    }

    // Update Player..
    if (player_ != nullptr)
        player_->Update(dt);

    // Update Player Bullets..
    playerBullets_.ForEach([&](Bullet& b)
    {
        b.Update(dt);
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

    if (formation_ != nullptr)
        formation_->Draw(renderer);
}
#include "World.h"
#include "Player.h"

void World::Add(std::unique_ptr<Entity> entity)
{
    entity->SetWorld(this);
    pending_.push_back(std::move(entity));
}

void World::AddPlayer(std::unique_ptr<Player> player)
{
    player_ = player.get();
    player_->onShoot = [this]
    {
        if (onPlayerShoot) onPlayerShoot();
    };
    Add(std::move(player));
}

void World::AddFormation(std::unique_ptr<Formation> formation)
{
    formation->SetWorld(this);
    formation_ = std::move(formation);
}

void World::SetPlayerPosition(Vector2<float> v)
{
    if (player_ != nullptr)
        player_->SetPosition(v);
}

void World::Clear()
{
    player_ = nullptr;
    pending_.clear();
    entities_.clear();

    if (formation_ != nullptr)
        formation_->Clear();
}

void World::HandleInput()
{
    for (const auto& e : entities_)
        e->HandleInput();
}

void World::Update(float dt)
{
    if (formation_ != nullptr)
    {
        // Update formation..
        formation_->Update(dt);

        // Update entities..
        for (const auto& e : entities_)
        {
            if (formation_->TryHit(e->GetRect()))
            {
                switch(e->GetType())
                {
                    case EntityType::Player:
                        if (onPlayerHit)
                            onPlayerHit();
                        break;
                    case EntityType::Bullet:
                        e->Damage(1);
                        if (onEnemyKilled)
                            onEnemyKilled();
                        break;
                    default:
                        break;
                }
            }
            else
            {
                e->Update(dt);
            }
        }
    }
    else
    {
        // Update entities..
        for (const auto& e : entities_)
        {
            e->Update(dt);
        }
    }


    // Remove dead entities..
    std::erase_if(entities_, [](auto& e){ return e->IsDead(); });
    
    // Adding pending_ entities..
    for (auto& e : pending_)
        entities_.push_back(std::move(e));
    pending_.clear();
}

void World::Draw(SDL_Renderer* renderer) const
{
    for (const auto& e : entities_)
        e->Draw(renderer);

    if (formation_ != nullptr)
        formation_->Draw(renderer);
}
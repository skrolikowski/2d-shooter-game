#include "World.h"

void World::Add(std::unique_ptr<Entity> entity)
{
    entity->SetWorld(this);
    pending_.push_back(std::move(entity));
}

void World::AddFormation(std::unique_ptr<Formation> formation)
{
    formation->SetWorld(this);
    formation_ = std::move(formation);
}

void World::Remove()
{
    //TODO:
}

void World::HandleInput()
{
    for (const auto& e : entities_)
        e->HandleInput();
}

void World::Update(float dt)
{
    if (formation_ != nullptr)
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
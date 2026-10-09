#include "Formation.h"
#include "World.h"

Formation::Formation(TextureManager& textures)
{
    numCols_ = 8;
    numRows_ = 5;
    columns_.reserve(numCols_);

    // --
    float formationWidth = numCols_ * ENTITY_SIZE + (numCols_ - 1) * INVADER_GAP;
    float startX = (SCREEN_WIDTH - formationWidth) / 2.f;
    float startY = 64.f;

    std::uniform_real_distribution<float> delay(1.f, 1.5f);
    bulletTimer_ = delay(rng_);

    for (int c = 0; c < numCols_; ++c)
    {
        float x = startX + c * (ENTITY_SIZE + INVADER_GAP);
        // float y = startY + r * (ENTITY_SIZE + INVADER_GAP);
        FormationColumn column{ numRows_, x, startY, textures };
        columns_.push_back(std::move(column));
    }
}

bool Formation::TryHit(const SDL_FRect& rect)
{
    for (auto& col : columns_)
        if (col.TryHit(rect))
            return true;

    return false;
}

void Formation::Clear()
{
    for (auto& col : columns_)
        col.Clear();
}

void Formation::Update(float dt)
{
    stepTimer_ -= dt;
    if (stepTimer_ <= 0)
    {
        StepFormation();
        int num_invaders = std::accumulate(columns_.begin(), columns_.end(), 0,
            [](int acc, FormationColumn& column){ return acc + column.Size(); });
        stepTimer_ = stepInterval_ - (stepTimer_ - 0);
        stepInterval_ = 0.02f + 0.78f * num_invaders / 55.0f;
    }

    bulletTimer_ -= dt;
    if (bulletTimer_ <= 0)
    {
        std::vector<const Entity*> lowestInvaders;
        for (const auto& col : columns_)
        {
            const Entity* inv = col.GetLowestInvader();
            if (inv != nullptr)
                lowestInvaders.push_back(inv);
        }

        if (lowestInvaders.size() > 0)
        {
            std::uniform_int_distribution<size_t> pick(0, lowestInvaders.size() - 1);
            const Entity* lowestInvader = lowestInvaders[pick(rng_)];
            float bx = lowestInvader->GetX() + lowestInvader->GetWidth() * 0.5;
            float by = lowestInvader->GetY() + 32.f;
            if (world_->SpawnInvaderBullet(Vector2{ bx, by }))
                if (onShoot)
                    onShoot();
        }
        
        // Reset timer..
        std::uniform_real_distribution<float> delay(1.f, 1.5f);
        bulletTimer_ = delay(rng_);
    }

}

void Formation::StepFormation()
{
    float dx = SCREEN_WIDTH / ENTITY_SIZE / 2.f;
    float dy = ENTITY_SIZE / 4.f;
    
    bool hitEdge = false;
    for (const auto& col : columns_)
        if (col.CheckForStepFormationHit(dx))
        {
            hitEdge = true;
            break;
        }

    for (auto& col : columns_)
        col.StepFormation(dx, dy, hitEdge);


    // for (const auto& e : invaders_)
    // {
    //     if (!e.IsDead())
    //     {
    //         float nextX = e.GetX() + dir_ * dx;
    //         if (nextX < dx || nextX + e.GetWidth() > SCREEN_WIDTH - dx)
    //         {
    //             hitEdge = true;
    //             dir_ = -dir_;
    //             frame_ ^= 1;
    //             break;
    //         }
    //     }
    // }

    // move invaders
    // for (auto& e : invaders_)
    // {
    //     if (hitEdge)
    //         e.SetY(e.GetY() + dy);
    //     else
    //         e.SetX(e.GetX() + dir_ * dx);
    // }
}

void Formation::Draw(SDL_Renderer* renderer) const
{
    for (auto& col : columns_)
        col.Draw(renderer);
}
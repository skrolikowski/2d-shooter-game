#include "Formation.h"

Formation::Formation(TextureManager& textures)
{
    int cols = 8;
    int rows = 5;
    invaders_.reserve(cols*rows);
    // --
    float formationWidth = cols * ENTITY_SIZE + (cols - 1) * INVADER_GAP;
    float startX = (SCREEN_WIDTH - formationWidth) / 2.f;
    float startY = 64.f;

    std::shared_ptr<Texture> texture = textures.Get("res/invader_blue.png");
    
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            float x = startX + c * (ENTITY_SIZE + INVADER_GAP);
            float y = startY + r * (ENTITY_SIZE + INVADER_GAP);

            Entity e(Vector2<float>{ x, y });
            e.SetTexture(texture);
            invaders_.push_back(std::move(e));
        }
    }
}

bool Formation::TryHit(const SDL_FRect& rect)
{
    for (Entity& e : invaders_)
    {
        SDL_FRect r{
            .x = e.GetX(),
            .y = e.GetY(),
            .w = e.GetWidth(),
            .h = e.GetHeight(),
        };

        if (SDL_HasRectIntersectionFloat(&r, &rect))
        {
            e.Damage(1);
            return true;
        }
    }

    return false;
}

void Formation::Clear()
{
    invaders_.clear();
}

void Formation::Update(float dt)
{
    // Update entities..
    // for (auto& e : invaders_)
    //     e.Update(dt);

    // Remove dead entities..
    std::erase_if(invaders_, [](auto& e){ return e.IsDead(); });
    
    stepTimer_ -= dt;
    if (stepTimer_ <= 0)
    {
        StepFormation();
        stepTimer_ = stepInterval_ - (stepTimer_ - 0);
        stepInterval_ = 0.02f + 0.78f * invaders_.size() / 55.0f;
        
    }
}

void Formation::StepFormation()
{
    float dx = SCREEN_WIDTH / ENTITY_SIZE / 2.f;
    float dy = ENTITY_SIZE / 4.f;
    bool hitEdge = false;

    for (const auto& e : invaders_)
    {
        if (!e.IsDead())
        {
            float nextX = e.GetX() + dir_ * dx;
            if (nextX < dx || nextX + e.GetWidth() > SCREEN_WIDTH - dx)
            {
                hitEdge = true;
                dir_ = -dir_;
                frame_ ^= 1;
                break;
            }
        }
    }

    // move invaders
    for (auto& e : invaders_)
    {
        if (hitEdge)
            e.SetY(e.GetY() + dy);
        else
            e.SetX(e.GetX() + dir_ * dx);
    }
}

void Formation::Draw(SDL_Renderer* renderer) const
{
    for (auto& e : invaders_)
        e.Draw(renderer);
}
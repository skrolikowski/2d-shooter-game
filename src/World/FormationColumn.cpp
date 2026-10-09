#include "FormationColumn.h"

FormationColumn::FormationColumn(int numRows, float x, float startY, TextureManager& textures)
{
    std::shared_ptr<Texture> texture = textures.Get("res/invader_blue.png");

    for (int r = 0; r < numRows; ++r)
    {
        float y = startY + r * (ENTITY_SIZE + INVADER_GAP);

        Entity e(Vector2<float>{ x, y });
        e.SetTexture(texture);
        invaders_.push_back(std::move(e));
    }
}

const Entity* FormationColumn::GetLowestInvader() const
{
    auto it = std::find_if(invaders_.rbegin(), invaders_.rend(), [](const Entity& inv){ return !inv.IsDead(); });
    if (it != invaders_.rend())
    {
        auto index = std::distance(it, invaders_.rend()) - 1;
        return &invaders_[index];
    }

    return nullptr;
}

bool FormationColumn::TryHit(const SDL_FRect& other)
{
    for (Entity& inv : invaders_)
    {
        if (inv.IsDead()) continue;
        
        SDL_FRect invRect = inv.GetRect();
        if (SDL_HasRectIntersectionFloat(&invRect, &other))
        {
            inv.Damage(1);
            return true;
        }
    }

    return false;
}

void FormationColumn::Clear()
{
    invaders_.clear();
}

void FormationColumn::Draw(SDL_Renderer* renderer) const
{
    for (auto& inv : invaders_)
    {
        if (inv.IsDead()) continue;

        inv.Draw(renderer);
    }
}

bool FormationColumn::CheckForStepFormationHit(float dx) const
{
    for (const auto& inv : invaders_)
    {
        if (inv.IsDead()) continue;

        float nextX = inv.GetX() + dir_ * dx;
        if (nextX < dx || nextX + inv.GetWidth() > SCREEN_WIDTH - dx)
        {
            return true;
        }
    }

    return false;
}

void FormationColumn::StepFormation(float dx, float dy, bool hitDetected)
{
    if (hitDetected)
        dir_ = -dir_;

    for (auto& inv : invaders_)
    {
        if (inv.IsDead()) continue;

        if (hitDetected)
            inv.SetY(inv.GetY() + dy);
        else
            inv.SetX(inv.GetX() + dir_ * dx);
    }
}
#include "FormationColumn.h"

#include <nlohmann/json.hpp>

FormationColumn::FormationColumn(Vector2<float> pos, const json &data)
{
    int numRows = static_cast<int>(data.size());

    for (int r = 0; r < numRows; ++r)
    {
        float px = pos.x;
        float py = pos.y + (static_cast<float>(r) * (ENTITY_SIZE + INVADER_GAP));

        Entity e(Vector2<float>{ px, py });

        std::string value = data.at(r).get<std::string>();
        if (value == "A")
        {
            e.SetType(InvaderType::A);
            e.SetTexture("enemyBlack1.png");
        }
        else if (value == "B")
        {
            e.SetType(InvaderType::B);
            e.SetTexture("enemyBlack2.png");
        }
        else if (value == "C")
        {
            e.SetType(InvaderType::C);
            e.SetTexture("enemyBlack3.png");
        }
        else
            e.SetHealth(0);

        invaders_.push_back(std::move(e));
    }
}

const Entity *FormationColumn::GetLowestInvader() const
{
    auto it = std::ranges::find_if(invaders_.rbegin(), invaders_.rend(),
                                   [](const Entity &inv) { return !inv.IsDead(); });
    if (it != invaders_.rend())
    {
        auto index = std::distance(it, invaders_.rend()) - 1;
        return &invaders_[index];
    }

    return nullptr;
}

Entity *FormationColumn::TryHit(const SDL_FRect &other)
{
    for (auto &inv : std::views::reverse(invaders_))
    {
        if (inv.IsDead())
            continue;

        SDL_FRect invRect = inv.GetRect();
        if (SDL_HasRectIntersectionFloat(&invRect, &other))
        {
            return &inv;
        }
    }

    return nullptr;
}

void FormationColumn::Clear()
{
    invaders_.clear();
}

int FormationColumn::Size() const
{
    return std::ranges::count_if(invaders_, [](auto &e) { return !e.IsDead(); });
}

void FormationColumn::Draw() const
{
    for (auto &inv : invaders_)
    {
        if (inv.IsDead())
            continue;
        inv.Draw();
    }
}

bool FormationColumn::CheckForStepFormationHit(float dx) const
{
    for (const auto &inv : invaders_)
    {
        if (inv.IsDead())
            continue;

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

    for (auto &inv : invaders_)
    {
        if (inv.IsDead())
            continue;

        if (hitDetected)
            inv.SetY(inv.GetY() + dy);
        else
            inv.SetX(inv.GetX() + dir_ * dx);
    }
}
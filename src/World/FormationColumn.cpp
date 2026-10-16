#include "FormationColumn.h"
#include "World/Invader.h"

#include <algorithm>
#include <nlohmann/json.hpp>

FormationColumn::FormationColumn(Vector2<float> pos, const json &data)
{
    int numRows = static_cast<int>(data.size());

    for (int r = 0; r < numRows; ++r)
    {
        float px = pos.x;
        float py = pos.y + (static_cast<float>(r) * (ENTITY_SIZE + INVADER_GAP));
        std::string value = data.at(static_cast<unsigned long>(r)).get<std::string>();

        Invader inv(Vector2<float>{ px, py });
        inv.SetHomePosition(Vector2<float>{ px, py });

        if (value == "A")
            inv.SetType(InvaderType::A);
        else if (value == "B")
            inv.SetType(InvaderType::B);
        else if (value == "C")
            inv.SetType(InvaderType::Diver);
        else
            inv.SetHealth(0);

        invaders_.push_back(std::move(inv));
    }
}

const Invader *FormationColumn::GetLowestInvader() const
{
    auto it = std::ranges::find_if(
        invaders_.rbegin(), invaders_.rend(), [](const Invader &inv) { return !inv.IsDead(); });
    if (it != invaders_.rend())
    {
        const auto index = std::distance(it, invaders_.rend()) - 1;
        return &invaders_[static_cast<unsigned long>(index)];
    }

    return nullptr;
}

Invader *FormationColumn::TryHit(const SDL_FRect &other)
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
    return static_cast<int>(std::ranges::count_if(invaders_, [](auto &e) { return !e.IsDead(); }));
}

void FormationColumn::Draw() const
{
    for (const auto &inv : invaders_)
    {
        if (inv.IsDead())
            continue;
        inv.Draw();
    }
}

bool FormationColumn::CheckForStepFormationHit(float dx) const
{
    return std::ranges::any_of(invaders_,
        [&](const auto &inv)
        {
            // Only Invaders "InFormation" will move with Formation
            if (inv.IsDead() or inv.GetState() != InvaderState::InFormation)
                return false;
            // --
            float nextX = inv.GetX() + (static_cast<float>(dir_) * dx);
            return (nextX < dx || nextX + inv.GetWidth() > SCREEN_WIDTH - dx);
        });
}

void FormationColumn::StepFormation(float dx, float dy, bool hitDetected)
{
    if (hitDetected)
        dir_ = -dir_;

    for (auto &inv : invaders_)
    {
        if (inv.IsDead())
            continue;

        // Next position..
        Vector2<float> nextPos = inv.GetPosition();

        if (inv.GetState() != InvaderState::InFormation)
            nextPos = inv.GetHomePosition();

        if (hitDetected)
            nextPos.y += dy;
        else
            nextPos.x += static_cast<float>(dir_) * dx;

        inv.SetHomePosition(nextPos);

        if (inv.GetState() == InvaderState::InFormation)
            inv.SetPosition(nextPos);
    }
}
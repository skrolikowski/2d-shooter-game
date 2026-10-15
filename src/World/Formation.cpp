#include "Formation.h"
#include "Event/EventBus.h"
#include "World.h"

#include <nlohmann/json.hpp>

void Formation::CreateWave(const json &data)
{
    bulletTimerRange_ = data.at("fire_rate");
    baseStepInterval_ = data.at("step_interval").get<float>();
    stepInterval_ = baseStepInterval_;

    const json &invaders = data.at("invaders");
    const int numCols = static_cast<int>(invaders.size());

    // --
    Clear();
    columns_.reserve(static_cast<unsigned long>(numCols));

    auto fNumCols = static_cast<float>(numCols);
    float formationWidth = (fNumCols * ENTITY_SIZE) + ((fNumCols - 1) * INVADER_GAP);
    float startX = (SCREEN_WIDTH - formationWidth) / 2.F;
    float startY = 64.F;

    for (int c = 0; c < numCols; ++c)
    {
        const json &invaderData = invaders.at(static_cast<unsigned long>(c));
        float x = startX + (static_cast<float>(c) * (ENTITY_SIZE + INVADER_GAP));
        FormationColumn column{ Vector2<float>{ x, startY }, invaderData };
        columns_.push_back(std::move(column));
    }

    numInvadersInit_ = GetNumInvaders();

    // --
    std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
    bulletTimer_ = delay(rng_);
}

Invader *Formation::TryHit(const SDL_FRect &rect)
{
    for (auto &col : columns_)
        if (Invader *invader = col.TryHit(rect))
            return invader;

    return nullptr;
}

void Formation::Clear()
{
    for (auto &col : columns_)
        col.Clear();
}

int Formation::GetNumInvaders() const
{
    return std::accumulate(columns_.begin(), columns_.end(), 0,
        [](int acc, const FormationColumn &column) { return acc + column.Size(); });
}

void Formation::Update(float dt)
{
    stepTimer_ -= dt;
    if (stepTimer_ <= 0)
    {
        StepFormation();
        // --
        const int numInvaders = GetNumInvaders();
        constexpr float minInterval = 0.02F;
        float ratio =
            numInvadersInit_ > 0 ? float(numInvaders) / static_cast<float>(numInvadersInit_) : 0.F;
        stepTimer_ = stepInterval_ - (stepTimer_ - 0);
        stepInterval_ = minInterval + ((baseStepInterval_ - minInterval) * ratio);
    }

    bulletTimer_ -= dt;
    if (bulletTimer_ <= 0)
    {
        std::vector<const Entity *> lowestInvaders;
        for (const auto &col : columns_)
        {
            const Entity *inv = col.GetLowestInvader();
            if (inv != nullptr)
                lowestInvaders.push_back(inv);
        }

        if (lowestInvaders.size() > 0)
        {
            std::uniform_int_distribution<size_t> pick(0, lowestInvaders.size() - 1);
            const Entity *lowestInvader = lowestInvaders[pick(rng_)];
            const auto bx =
                static_cast<float>(lowestInvader->GetX() + (lowestInvader->GetWidth() * 0.5));
            const float by = lowestInvader->GetY() + 32.F;
            Vector2 bPos = Vector2{ bx, by };

            if (world_->SpawnInvaderBullet(bPos))
            {
                ShotFired ev;
                ev.pos = bPos;
                ev.isPlayerFire = false;
                EventBus::GetInstance().Push(ev);
            }
        }

        // Reset timer..
        std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
        bulletTimer_ = delay(rng_);
    }
}

void Formation::StepFormation()
{
    float dx = SCREEN_WIDTH / ENTITY_SIZE / 2.F;
    float dy = ENTITY_SIZE / 4.F;

    bool hitEdge = false;
    for (const auto &col : columns_)
        if (col.CheckForStepFormationHit(dx))
        {
            hitEdge = true;
            break;
        }

    for (auto &col : columns_)
        col.StepFormation(dx, dy, hitEdge);
}

void Formation::Draw() const
{
    for (const auto &col : columns_)
        col.Draw();
}
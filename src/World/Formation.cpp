#include "Formation.h"
#include "World.h"
#include "Util/EventQueue.h"

#include <nlohmann/json.hpp>

void Formation::CreateWave(const json& data)
{
    bulletTimerRange_ = data.at("fire_rate");
    baseStepInterval_ = data.at("step_interval").get<float>();
    stepInterval_ = baseStepInterval_;

    const json& invaders = data.at("invaders");
    int numCols = invaders.size();

    // --
    Clear();
    columns_.reserve(numCols);

    float formationWidth = numCols * ENTITY_SIZE + (numCols - 1) * INVADER_GAP;
    float startX = (SCREEN_WIDTH - formationWidth) / 2.f;
    float startY = 64.f;

    for (int c = 0; c < numCols; ++c)
    {
        const json& invaderData = invaders.at(c);
        float x = startX + c * (ENTITY_SIZE + INVADER_GAP);
        FormationColumn column{ x, startY, invaderData };
        columns_.push_back(std::move(column));
    }

    numInvadersInit_ = GetNumInvaders();

    // --
    std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
    bulletTimer_ = delay(rng_);
}

Entity* Formation::TryHit(const SDL_FRect& rect)
{
    for (auto& col : columns_)
        if (Entity* invader = col.TryHit(rect))
            return invader;

    return nullptr;
}

void Formation::Clear()
{
    for (auto& col : columns_)
        col.Clear();
}

int Formation::GetNumInvaders() const
{
    return std::accumulate(columns_.begin(), columns_.end(), 0,
        [](int acc, const FormationColumn& column){ return acc + column.Size(); });
}

void Formation::Update(float dt)
{
    stepTimer_ -= dt;
    if (stepTimer_ <= 0)
    {
        StepFormation();
        // --
        const int numInvaders = GetNumInvaders();
        constexpr float minInterval = 0.02f;
        float ratio = numInvadersInit_ > 0 ? float(numInvaders) / numInvadersInit_ : 0.f;
        stepTimer_ = stepInterval_ - (stepTimer_ - 0);
        stepInterval_ = minInterval + (baseStepInterval_ - minInterval) * ratio;
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
            Vector2 bPos = Vector2{ bx, by };
            
            if (world_->SpawnInvaderBullet(bPos))
                EventQueue::GetInstance().Push(ShotFired{ bPos , false });
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
}

void Formation::Draw() const
{
    for (auto& col : columns_)
        col.Draw();
}
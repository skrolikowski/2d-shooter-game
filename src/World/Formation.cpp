#include "Formation.h"
#include "App.h"
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

    Clear();
    columns_.reserve(static_cast<unsigned long>(numCols));

    auto fNumCols = static_cast<float>(numCols);
    float formationWidth = (fNumCols * ENTITY_SIZE) + ((fNumCols - 1) * INVADER_GAP);
    float startX = (SCREEN_WIDTH - formationWidth) / 2.F;
    float startY = FORMATION_Y_START;

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
    bulletTimer_ = delay(App::RNG);
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

void Formation::UpdateDiverInvader(float dt)
{
    diveTimer_ += dt;
    if (diveTimer_ >= diveTimerDelay_)
    {
        SelectRandomDiverInvader();
        diveTimer_ = 0.F; // reset;
    }
}

void Formation::SelectRandomDiverInvader()
{
    std::vector<Invader *> candidates;

    ForEachInvader(
        [&](Invader &invader)
        {
            if (invader.GetState() == InvaderState::InFormation &&
                invader.GetType() == InvaderType::Diver)
            {
                candidates.push_back(&invader);
            }
        });

    if (candidates.empty())
        return;

    // ..
    std::uniform_int_distribution<std::size_t> select(0, candidates.size() - 1);
    std::size_t randIndex = select(App::RNG);

    Invader *inv = candidates[randIndex];
    inv->SetState(InvaderState::Diving);

    bool invaderOnLeft = inv->GetCenter().x < (SCREEN_WIDTH / 2.F);
    float padX = 16.F;
    float padY = 160.F;
    float px1 = invaderOnLeft ? padX : SCREEN_WIDTH + padX;
    float px2 = invaderOnLeft ? SCREEN_WIDTH + padX : padX;

    // control points
    Vector2<float> p0 = inv->GetPosition();
    Vector2<float> p1 = Vector2<float>{ px1, SCREEN_HEIGHT + padY };
    Vector2<float> p2 = Vector2<float>{ px2, SCREEN_HEIGHT + padY };
    Vector2<float> p3 = Vector2<float>{ px2, p0.y };
    inv->GetDivePath().Start(p0, p1, p2, p3, INVADER_DIVE_DURATION);
}

void Formation::UpdateStepFormation(float dt)
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
}

void Formation::UpdateSpawnBullet(float dt)
{
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
            const Entity *lowestInvader = lowestInvaders[pick(App::RNG)];
            const auto bx =
                static_cast<float>(lowestInvader->GetX() + (lowestInvader->GetWidth() * 0.5));
            const float by = lowestInvader->GetY() + 32.F;
            Vector2 bPos = Vector2{ bx, by };

            if (world_->SpawnEnemyBullet(bPos))
            {
                ShotFired ev;
                ev.pos = bPos;
                ev.isPlayerFire = false;
                EventBus::GetInstance().Push(ev);
            }
        }

        // Reset timer..
        std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
        bulletTimer_ = delay(App::RNG);
    }
}

void Formation::Update(float dt)
{
    UpdateDiverInvader(dt);
    UpdateStepFormation(dt);
    UpdateSpawnBullet(dt);

    ForEachInvader(
        [&](Invader &invader)
        {
            if (!invader.IsDead())
                invader.Update(dt);
        });
}

void Formation::StepFormation()
{
    float dx = FORMATION_DX_STEP;
    float dy = FORMATION_DY_STEP;

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
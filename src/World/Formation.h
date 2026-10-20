#ifndef FORMATION_H
#define FORMATION_H

#include <array>
#include <cassert>
#include <nlohmann/json_fwd.hpp>
#include <random>
#include <vector>
using json = nlohmann::json;

#include "Entity.h"
#include "FormationColumn.h"
#include "Util/Texture.h"

enum class WaveType : std::uint8_t
{
    Invaders,
    Boss
};

class World; // forward declaration

class Formation
{
  public:
    void CreateWave(const json &data);
    void SetWorld(World *world) { world_ = world; }
    void SelectRandomDiverInvader();
    [[nodiscard]] int GetNumInvaders() const;
    Invader *TryHit(const SDL_FRect &rect);
    void StepFormation();
    [[nodiscard]] bool IsDead() const { return GetNumInvaders() == 0; }
    void Clear();
    void Update(float dt);
    void UpdateDiverInvader(float dt);
    void UpdateStepFormation(float dt);
    void UpdateSpawnBullet(float dt);
    void Draw() const;

    template <typename F> void ForEachInvader(F &&f)
    {
        for (auto &column : columns_)
        {
            column.ForEach(f);
        }
    }

  private:
    World *world_ = nullptr;
    std::vector<FormationColumn> columns_;

    std::mt19937 rng_{ std::random_device{}() };
    int numInvadersInit_ = 0;
    float stepTimer_ = 0.F;
    float stepInterval_ = 0.F;
    float baseStepInterval_ = 0.F;

    float diveTimer_ = 0.F;
    float diveTimerDelay_ = INVADER_DIVE_DELAY;

    float bulletTimer_ = 0.F;
    std::array<float, 2> bulletTimerRange_ = { 1.F, 1.5F };
};

#endif // FORMATION_H
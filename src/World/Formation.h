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

class World; // forward declaration

class Formation
{
  public:
    void CreateWave(const json &data);
    void SetWorld(World *world) { world_ = world; }
    [[nodiscard]] int GetNumInvaders() const;
    Invader *TryHit(const SDL_FRect &rect);
    void StepFormation();
    [[nodiscard]] bool IsDead() const { return GetNumInvaders() == 0; }
    void Clear();
    void Update(float dt);
    void Draw() const;

  private:
    World *world_ = nullptr;
    std::vector<FormationColumn> columns_;

    std::mt19937 rng_{ std::random_device{}() };
    int numInvadersInit_ = 0;
    float stepTimer_ = 0.F;
    float stepInterval_ = 0.F;
    float baseStepInterval_ = 0.F;

    float bulletTimer_ = 0.F;
    std::array<float, 2> bulletTimerRange_ = { 1.F, 1.5F };
};

#endif // FORMATION_H
#ifndef FORMATION_H
#define FORMATION_H

#include <cassert>
#include <array>
#include <random>
#include <vector>
#include <numeric>
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

#include "Defs.h"
#include "FormationColumn.h"
#include "Util/TextureManager.h"

class World; // forward declaration

class Formation
{
public:
    void CreateWave(const json& data, TextureManager& textures);
    void SetWorld(World* world) { world_ = world; }
    int GetNumInvaders() const;
    bool TryHit(const SDL_FRect& rect);
    void StepFormation();
    void Clear();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;

    std::function<void()> onShoot;

private:
    World* world_ = nullptr;
    std::vector<FormationColumn> columns_;

    std::mt19937 rng_{ std::random_device{}() }; 
    int numInvadersInit_ = 0;
    float stepTimer_ = 0.f;
    float stepInterval_ = 0.f;
    float baseStepInterval_ = 0.f;

    float bulletTimer_ = 0.f;
    std::array<float, 2> bulletTimerRange_ = { 1.f, 1.5f };

};

#endif // FORMATION_H
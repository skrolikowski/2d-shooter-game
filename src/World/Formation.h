#ifndef FORMATION_H
#define FORMATION_H

#include <random>
#include <vector>

#include "Defs.h"
#include "FormationColumn.h"
#include "Util/TextureManager.h"

class World; // forward declaration

class Formation
{
public:
    explicit Formation(TextureManager& textures);
    ~Formation() {}

    bool TryHit(const SDL_FRect& rect);
    void SetWorld(World* world) { world_ = world; }
    void StepFormation();
    void Clear();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;

    std::function<void()> onShoot;

private:
    World* world_;
    // std::vector<Entity> invaders_;
    std::vector<FormationColumn> columns_;

    int numRows_;
    int numCols_;

    // int dir_ = 1;
    // int frame_ = 0;
    std::mt19937 rng_{ std::random_device{}() }; 
    float stepTimer_ = 0.f;
    float stepInterval_ = 0.8f;

    float bulletTimer_ = 0.f;

    // const Entity* getRandomLowestInvader() const;

};

#endif // FORMATION_H
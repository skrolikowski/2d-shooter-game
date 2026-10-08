#ifndef FORMATION_H
#define FORMATION_H

#include <vector>

#include "Defs.h"
#include "TextureManager.h"
#include "Entity.h"

class World; // forward declaration

class Formation
{
public:
    explicit Formation(TextureManager& textures);
    ~Formation() {}

    bool TryHit(const SDL_FRect& rect);
    void SetWorld(World* world) { world_ = world; }
    void StepFormation();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;

private:
    World* world_;
    std::vector<Entity> invaders_;

    int dir_ = 1;
    int frame_ = 0;
    float stepTimer_ = 0.f;
    float stepInterval_ = 0.8f;

};

#endif // FORMATION_H
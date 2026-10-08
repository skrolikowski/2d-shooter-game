#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <functional>

#include "Formation.h"
#include "TextureManager.h"
#include "Entity.h"

class World
{
public:
    World() = default;
    ~World() { }

    void Add(std::unique_ptr<Entity> entity);
    void AddFormation(std::unique_ptr<Formation> formation);
    void Clear();

    std::function<void()> onPlayerHit;
    std::function<void()> onEnemyKilled;

    void HandleInput();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;

private:
    std::vector<std::unique_ptr<Entity>> pending_;
    std::vector<std::unique_ptr<Entity>> entities_;
    std::unique_ptr<Formation> formation_;
};

#endif // WORLD_H

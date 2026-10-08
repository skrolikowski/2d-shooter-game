#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <functional>

#include "Entity.h"
#include "Formation.h"
#include "Util/TextureManager.h"
#include "Util/Vector2.h"

class Player; // forward declaration

class World
{
public:
    World() = default;
    ~World() { }

    void Add(std::unique_ptr<Entity> entity);
    void AddPlayer(std::unique_ptr<Player> player);
    void AddFormation(std::unique_ptr<Formation> formation);
    void Clear();

    std::function<void()> onPlayerShoot;
    std::function<void()> onPlayerHit;
    std::function<void()> onEnemyKilled;

    void HandleInput();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;
    void SetPlayerPosition(Vector2<float> v);

private:
    std::vector<std::unique_ptr<Entity>> pending_;
    std::vector<std::unique_ptr<Entity>> entities_;
    std::unique_ptr<Formation> formation_ = nullptr;
    Player* player_ = nullptr;

};

#endif // WORLD_H

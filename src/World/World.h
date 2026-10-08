#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <functional>

#include "Bullet.h"
#include "Entity.h"
#include "Formation.h"
#include "Util/Pool.h"
#include "Util/TextureManager.h"
#include "Util/Vector2.h"

class Player; // forward declaration

class World
{
public:
    World() = default;
    ~World();

    void AddPlayer(std::unique_ptr<Player> player);
    void AddFormation(std::unique_ptr<Formation> formation);
    void Clear();

    bool SpawnPlayerBullet(Vector2<float> v);
    void SetPlayerPosition(Vector2<float> v);

    std::function<void()> onPlayerShoot;
    std::function<void()> onPlayerHit;
    std::function<void()> onEnemyKilled;

    void HandleInput();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;
    
private:
    std::unique_ptr<Formation> formation_ = nullptr;
    std::unique_ptr<Player> player_ = nullptr;
    Pool<Bullet, 3> playerBullets_;

};

#endif // WORLD_H

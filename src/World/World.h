#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <memory>
#include <vector>
#include <functional>

#include "Bunker.h"
#include "Bullet.h"
#include "Formation.h"
#include "Util/Pool.h"
#include "Util/Vector2.h"

class Player; // forward declaration

class World
{
public:
    World();
    ~World();

    void Clear();
    bool SpawnBunker(Vector2<float> pos);
    bool SpawnInvaderBullet(Vector2<float> pos);
    bool SpawnPlayerBullet(Vector2<float> pos);
    void SetPlayerPosition(Vector2<float> pos);

    std::function<void()> onPlayerShoot;
    std::function<void()> onInvaderShoot;
    std::function<void()> onPlayerHit;
    std::function<void()> onInvaderKilled;
    std::function<void()> onFormationKilled;

    Formation* GetFormation() { return formation_.get(); }

    void HandleInput();
    void Update(float dt);
    void Draw(SDL_Renderer* renderer) const;
    
private:
    std::unique_ptr<Formation> formation_ = nullptr;
    std::unique_ptr<Player> player_ = nullptr;
    Pool<Bullet, 3> playerBullets_;
    Pool<Bullet, 2> invaderBullets_;
    Pool<Bunker, 3> bunkers_;

};

#endif // WORLD_H

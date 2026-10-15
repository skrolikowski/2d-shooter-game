#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <memory>

#include "Bullet.h"
#include "Bunker.h"
#include "Formation.h"
#include "PowerUp.h"
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
    void GivePlayerPowerUp(PowerUpType powerUpType);
    void RemovePlayerPowerUp(PowerUpType powerUpType);
    bool SpawnPowerUp(Vector2<float> pos, PowerUpType powerUpType);
    bool SpawnInvaderBullet(Vector2<float> pos);
    bool SpawnPlayerBullet(Vector2<float> pos);
    void DamagePlayer(int amount);
    void SpawnExplosion(Vector2<float> pos);
    void SetPlayerPosition(Vector2<float> pos);
    [[nodiscard]] bool PlayerHasPowerUp(PowerUpType powerUpType) const;

    Formation *GetFormation() { return formation_.get(); }

    void HandleInput();
    void Update(float dt);
    void Draw() const;

  private:
    std::unique_ptr<Formation> formation_ = nullptr;
    std::unique_ptr<Player> player_ = nullptr;
    Pool<Bullet, PLAYER_BULLET_POOL_MAX> playerBullets_;
    Pool<Bullet, 2> invaderBullets_;
    Pool<Bunker, 3> bunkers_;
    Pool<PowerUp, POWERUP_WORLD_LIMIT> powerUps_;
    Pool<Particle, EXPLOSION_PARTICLES> particles_;
};

#endif // WORLD_H

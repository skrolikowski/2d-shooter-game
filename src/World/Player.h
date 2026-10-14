#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
  public:
    Player();
    using Entity::Entity; // inherit the Entity(const Vector2<float>&) constructor

    void HandleInput() override;
    void Update(float dt) override;

  private:
    bool isShooting = false;
    float shootTimer = 0.F;
};

#endif // PLAYER_H
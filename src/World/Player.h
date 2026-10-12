#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>

#include "Defs.h"
#include "Entity.h"
#include "Util/Pool.h"
#include "Util/Input.h"

class Player : public Entity
{
public:
    Player();
    using Entity::Entity;   // inherit the Entity(const Vector2<float>&) constructor

    void HandleInput() override;
    void Update(float dt) override;

private:
    bool isShooting = false;
    float shootTimer = 0.f;

};

#endif // PLAYER_H
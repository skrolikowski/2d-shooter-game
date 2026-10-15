#ifndef POWER_UP_H
#define POWER_UP_H

#include "Entity.h"

class PowerUp : public Entity
{
  public:
    using Entity::Entity;

    void SetType(PowerUpType type);
    [[nodiscard]] PowerUpType GetType() const { return type_; }

  private:
    PowerUpType type_{};
};

#endif // POWER_UP_H

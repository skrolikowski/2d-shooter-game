#ifndef PLAYER_H
#define PLAYER_H

#include <array>

#include "Entity.h"

class Player : public Entity
{
  public:
    Player();
    using Entity::Entity; // inherit the Entity(const Vector2<float>&) constructor

    [[nodiscard]] bool HasPowerUp(PowerUpType powerUpType) const;
    void GainPowerUp(PowerUpType powerUpType);
    void LosePowerUp(PowerUpType powerUpType);
    void UpdatePowerUps(float dt);
    void HandleInput() override;
    void Update(float dt) override;
    void Draw() const override;

  private:
    bool isShooting = false;
    float shootTimer = 0.F;

    std::array<float, static_cast<std::size_t>(PowerUpType::Count)> powerUpTimers_{};
};

#endif // PLAYER_H
#ifndef BOSS_H
#define BOSS_H

#include <array>
#include <random>

#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

#include "Entity.h"

class Boss : public Entity
{
  public:
    using Entity::Entity;
    Boss(const Vector2<float> &pos, const json &data);

    void SetState(BossState state) { state_ = state; }
    [[nodiscard]] BossState GetState() const { return state_; };

    void Update(float dt) override;
    void Draw() const override;

  private:
    void updateSpawnBullet(float dt);

    BossState state_{};
    DivePath divePath_;
    float diveChance_ = 0.2F;

    float bulletTimer_ = 0.F;
    std::array<float, 2> bulletTimerRange_ = { 0.5F, 1.F };
};

#endif // BOSS_H
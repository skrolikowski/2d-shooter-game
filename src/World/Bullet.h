#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "Util/Tween.h"

class Bullet : public Entity
{
  public:
    Bullet();

    [[nodiscard]] const SDL_FRect &GetSweep() const { return sweep_; }
    void SetSweep(const SDL_FRect &s) { sweep_ = s; }

    void SetTravelTween(TravelTween tween) { tween_ = tween; }
    void Update(float dt) override;
    void UpdateAndSweep(float dt);

  private:
    SDL_FRect sweep_{};
    TravelTween tween_{};
};

#endif // BULLET_H
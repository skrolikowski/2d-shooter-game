#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity
{
  public:
    Bullet();

    [[nodiscard]] const SDL_FRect &GetSweep() const { return sweep_; }
    void SetSweep(const SDL_FRect &s) { sweep_ = s; }

  private:
    SDL_FRect sweep_{};
};

#endif // BULLET_H
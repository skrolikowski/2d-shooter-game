#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity
{
public:
    Bullet();

    const SDL_FRect& GetSweep() const { return sweep_; }
    void SetSweep(const SDL_FRect& s) { sweep_ = s; }

    float GetWidth() const override { return 8.f; }
    float GetHeight() const override { return 8.f; }

    void Draw(SDL_Renderer* renderer) const override;

private:
    SDL_FRect sweep_{};

};

#endif // BULLET_H
#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity
{
public:
    Bullet();

    float GetWidth() const override { return 8.f; }
    float GetHeight() const override { return 8.f; }

    void Draw(SDL_Renderer* renderer) const override;
};

#endif // BULLET_H
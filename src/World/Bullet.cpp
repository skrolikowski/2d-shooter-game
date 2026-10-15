#include "Bullet.h"

Bullet::Bullet()
{
    textureName_ = "laserBlue04.png";
    speed_ = 300.F;
    width_ = 6.F;
    height_ = 14.F;
}

void Bullet::UpdateAndSweep(float dt)
{
    /**
     * Dev Notes:
     * Create a SDL_FRect of the full Bullet path to combat Collision Tunneling
     */
    SDL_FRect before;
    SDL_FRect after;
    before = GetRect();
    Entity::Update(dt);
    after = GetRect();
    SDL_GetRectUnionFloat(&before, &after, &sweep_);
}
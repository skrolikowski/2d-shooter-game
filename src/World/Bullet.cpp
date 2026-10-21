#include "Bullet.h"

Bullet::Bullet()
{
    textureName_ = "laserBlue04.png";
    speed_ = 300.F;
    width_ = 6.F;
    height_ = 14.F;
}

void Bullet::Update(float dt)
{
    Entity::Update(dt);

    // check if out of bounds..
    float w = GetWidth();
    float h = GetHeight();
    if (pos_.x + w < 0 || pos_.x > SCREEN_WIDTH || pos_.y + h < 0 || pos_.y > SCREEN_HEIGHT)
        health_ = 0; // dead; out of bounds
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
    if (hasTween_)
        speed_ = tween_.Step(dt);
    Update(dt);
    after = GetRect();
    SDL_GetRectUnionFloat(&before, &after, &sweep_);
}
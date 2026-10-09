#include "Bullet.h"

Bullet::Bullet()
{
    speed_ = 300.f;
    width_ = 4.f;
    height_ = 4.f;
}

void Bullet::Draw(SDL_Renderer* renderer) const
{
    SDL_FRect rect{ pos_.x, pos_.y, width_, height_ };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

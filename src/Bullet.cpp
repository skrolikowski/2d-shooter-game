#include "Bullet.h"

Bullet::Bullet()
{
    type_ = EntityType::Bullet;
    speed_ = 500.f;
    dir_.y = -1;
}

void Bullet::Draw(SDL_Renderer* renderer) const
{
    SDL_FRect rect{ pos_.x, pos_.y, 8, 8 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

#include "Entity.h"


void Entity::Damage(int amount)
{
    health_ -= amount;
    health_ = std::max(0, health_);
}

void Entity::Update(float dt)
{
    vel_ = dir_ * speed_ * dt;
    pos_ += vel_;

    //TODO: check collision
    
    //TODO: check if out of bounds
    float w = GetWidth();
    float h = GetHeight();
    if (pos_.x + w < 0 || pos_.x > SCREEN_WIDTH || pos_.y + h < 0 || pos_.y > SCREEN_HEIGHT)
        health_ = 0; // dead; out of bounds
}

void Entity::Draw(SDL_Renderer* renderer) const
{
    if (texture_)
    {
        float sx = width_ / texture_->GetWidth();
        float sy = height_ / texture_->GetHeight();
        texture_->Draw(renderer, pos_.x, pos_.y, sx, sy);
    }
}
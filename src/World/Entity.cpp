#include "Entity.h"
#include "Util/Texture.h"

Vector2<float> Entity::GetCenter() const
{
    return { pos_.x + (width_ / 2.F), pos_.y + (height_ / 2.F) };
}

void Entity::Damage(int amount)
{
    health_ -= amount;
    health_ = std::max(0, health_);
}

void Entity::Update(float dt)
{
    vel_ = dir_.Normalize() * speed_ * dt;
    pos_ += vel_;
}

void Entity::Draw() const
{
    if (!textureName_.empty())
    {
        const Frame *frame = Texture::GetFrame(textureName_);
        if (frame != nullptr)
        {
            float sx = width_ / frame->rect.w;
            float sy = height_ / frame->rect.h;
            Texture::Draw(frame, pos_.x, pos_.y, sx, sy);
        }
    }
}
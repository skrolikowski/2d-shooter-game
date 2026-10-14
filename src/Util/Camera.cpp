#include <algorithm>

#include "Camera.h"

Camera::Camera()
{
    Listen<PlayerHit>([this](const PlayerHit &) { trauma_ = std::min(1.F, trauma_ + .5F); });
}

void Camera::Update(float dt)
{
    if (trauma_ <= 0.F)
        return;

    trauma_ = std::max(0.F, trauma_ - (dt * 1.5F));
    float shake = trauma_ * trauma_; // squared = punchy, then a soft falloff

    Offset = {
        maxOffset_ * shake * rand(-1, 1),
        maxOffset_ * shake * rand(-1, 1),
    };
}

float Camera::rand(float min, float max)
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(rng_);
}
#include <algorithm>

#include "Camera.h"
#include "Defs.h"
#include "Util/Utility.h"

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
        MAX_CAMERA_SHAKE * shake * Rand(-1, 1),
        MAX_CAMERA_SHAKE * shake * Rand(-1, 1),
    };
}

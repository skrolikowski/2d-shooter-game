#pragma once

#include <cstdlib>

#include "Event/EventListener.h"
#include "Util/Vector2.h"

class Camera : public EventListener
{
  public:
    Camera();
    void Update(float dt);

    inline static Vector2<float> Offset{};

  private:
    float trauma_ = 0.F;
};

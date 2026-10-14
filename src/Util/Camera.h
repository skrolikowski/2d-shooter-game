#pragma once

#include <cstdlib>
#include <random>

#include "Event/EventListener.h"
#include "Util/Vector2.h"

class Camera : public EventListener
{
  public:
    Camera();
    void Update(float dt);

    inline static Vector2<float> Offset{};

  private:
    std::mt19937 rng_{ std::random_device{}() };
    float trauma_ = 0.F;
    const float maxOffset_ = 5.F;

    inline float rand(float min, float max);
};

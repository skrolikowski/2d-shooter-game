#include <algorithm>

#include "Easing.h"

/**
 *  `float (*)(float)` is 8 bytes, has no heap allocation, and copies for free,
 *  while std::function is bigger and heavier..
 */
using EaseFn = float (*)(float);

struct TravelTween
{
    float from = 300.F;
    float to = 300.F;
    float duration = 0.F; // 0 = constant speed
    float elapsed = 0.F;
    EaseFn ease = Easing::Linear;

    float Step(float dt)
    {
        if (duration <= 0.F)
            return to;
        elapsed = std::min(duration, elapsed + dt);
        return from + ((to - from) * ease(elapsed / duration));
    }
};

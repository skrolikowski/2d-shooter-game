#ifndef UTILITY_H
#define UTILITY_H

#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

#include "App.h"

enum class TextAlign : uint8_t
{
    Left,
    Center,
    Right
};

inline void DrawText(TTF_Text *text, float x, float y, TextAlign align = TextAlign::Left)
{
    int w = 0;
    int h = 0;
    TTF_GetTextSize(text, &w, &h);
    if (align == TextAlign::Center)
        x -= static_cast<float>(w) / 2.F;
    else if (align == TextAlign::Right)
        x -= static_cast<float>(w);
    TTF_DrawRendererText(text, x, y);
}

inline SDL_FColor Lerp(SDL_FColor a, SDL_FColor b, float t)
{
    return {
        a.r + ((b.r - a.r) * t),
        a.g + ((b.g - a.g) * t),
        a.b + ((b.b - a.b) * t),
        a.a + ((b.a - a.a) * t),
    };
}

inline float Rand(float min, float max)
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(App::RNG);
}

#endif // UTILITY_H

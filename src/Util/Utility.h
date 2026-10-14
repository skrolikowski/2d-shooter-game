#ifndef UTILITY_H
#define UTILITY_H

#include <SDL3_ttf/SDL_ttf.h>

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

#endif // UTILITY_H

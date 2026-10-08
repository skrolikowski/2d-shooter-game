#ifndef UTILITY_H
#define UTILITY_H

#include <SDL3_ttf/SDL_ttf.h>

enum class TextAlign { Left, Center, Right };

static void DrawText(TTF_Text* text, float x, float y, TextAlign align = TextAlign::Left)
{
    int w = 0, h = 0;
    TTF_GetTextSize(text, &w, &h);
    if (align == TextAlign::Center) x -= w / 2.f;
    else if (align == TextAlign::Right) x -= w;
    TTF_DrawRendererText(text, x, y);
}

#endif // UTILITY_H

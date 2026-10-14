#include <algorithm>
#include <cmath>

#include "Bunker.h"
#include "Util/Texture.h"

std::optional<SDL_FPoint> Bunker::FindImpact(const SDL_FRect &swept, bool fromBelow) const
{
    SDL_FRect rect{
        .x = pos_.x,
        .y = pos_.y,
        .w = NumCols,
        .h = NumRows,
    };

    if (!SDL_HasRectIntersectionFloat(&rect, &swept))
        return std::nullopt;

    // --
    // --
    int r0 = std::max(0, int(std::floor(swept.y - pos_.y)));
    int r1 = std::min(NumRows, int(std::ceil(swept.y + swept.h - pos_.y)));
    int c0 = std::max(0, int(std::floor(swept.x - pos_.x)));
    int c1 = std::min(NumCols, int(std::ceil(swept.x + swept.w - pos_.x)));

    if (fromBelow)
    {
        for (int row = r1 - 1; row >= r0; --row) // search bottom → top
            for (int col = c0; col < c1; ++col)
                if (cells_[row][col])
                    return SDL_FPoint{ pos_.x + col, pos_.y + row };
    }
    else
    {
        for (int row = r0; row < r1; ++row) // search top → bottom
            for (int col = c0; col < c1; ++col)
                if (cells_[row][col])
                    return SDL_FPoint{ pos_.x + col, pos_.y + row };
    }

    return std::nullopt;
}

void Bunker::Erode(const SDL_FPoint &point)
{
    const int radius = BUNKER_BLAST_RADIUS;
    const float cy = point.y - pos_.y;
    const float cx = point.x - pos_.x;
    const int r0 = std::max(0, int(std::floor(cy - radius)));
    const int r1 = std::min(NumRows, int(std::ceil(cy + radius)) + 1);
    const int c0 = std::max(0, int(std::floor(cx - radius)));
    const int c1 = std::min(NumCols, int(std::ceil(cx + radius)) + 1);

    for (int row = r0; row < r1; ++row)
    {
        for (int col = c0; col < c1; ++col)
        {
            int dr = row - static_cast<int>(cy);
            int dc = col - static_cast<int>(cx);
            if ((dr * dr) + (dc * dc) <= radius * radius)
                cells_[row][col] = false;
        }
    }
}

void Bunker::Draw() const
{
    SDL_SetRenderDrawColor(Texture::Renderer, 255, 255, 255, 255);

    for (std::size_t r = 0; r < NumRows; ++r)
    {
        for (std::size_t c = 0; c < NumCols; ++c)
        {
            if (cells_[r][c])
            {
                SDL_FRect rect{ pos_.x + float(c), pos_.y + float(r), 1, 1 };
                SDL_RenderFillRect(Texture::Renderer, &rect);
            }
        }
    }
}
#include "Bunker.h"

std::optional<SDL_FPoint> Bunker::FindImpact(const SDL_FRect& swept) const
{
    SDL_FRect rect{
        .x = pos_.x,
        .y = pos_.y,
        .w = NumCols,
        .h = NumRows,
    };

    if (SDL_HasRectIntersectionFloat(&rect, &swept))
    {
        int r0 = std::max(0,       int(std::floor(swept.y - pos_.y)));
        int r1 = std::min(NumRows, int(std::ceil (swept.y + swept.h - pos_.y)));
        int c0 = std::max(0,       int(std::floor(swept.x - pos_.x)));
        int c1 = std::min(NumCols, int(std::ceil (swept.x + swept.w - pos_.x)));
    
        for (int row = r1 - 1; row >= r0; --row)  // search bottom → top
            for (int col = c0; col < c1; ++col)
                if (cells_[row][col])
                    return SDL_FPoint{ pos_.x + col, pos_.y + row };
    }
    
    return std::nullopt;
}

void Bunker::Erode(const SDL_FPoint& point)
{
    int radius = BUNKER_BLAST_RADIUS;
    float cy = point.y - pos_.y;
    float cx = point.x - pos_.x;
    int r0 = std::max(0,       int(std::floor(cy - radius)));
    int r1 = std::min(NumRows, int(std::ceil (cy + radius)) + 1);
    int c0 = std::max(0,       int(std::floor(cx - radius)));
    int c1 = std::min(NumCols, int(std::ceil (cx + radius)) + 1);

    for (int row = r0; row < r1; ++row)
    {
        for (int col = c0; col < c1; ++col)
        {
            int dr = row - cy;
            int dc = col - cx;
            if (dr*dr + dc*dc <= radius*radius)
                cells_[row][col] = false;

        }
    }
}

void Bunker::Draw(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int r = 0; r < NumRows; ++r)
    {
        for (int c = 0; c < NumCols; ++c)
        {
            if (cells_[r][c])
            {
                SDL_FRect rect{ pos_.x + c, pos_.y + r, 1, 1 };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}
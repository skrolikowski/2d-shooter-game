#include "Bunker.h"

bool Bunker::TryHit(const SDL_FRect& other)
{
    SDL_FRect rect{
        .x = pos_.x,
        .y = pos_.y,
        .w = NumCols,
        .h = NumRows,
    };

    if (SDL_HasRectIntersectionFloat(&rect, &other))
    {
        /**
         * Pixel collision testing:
         * Cycle through each Bullet pixel and test if cell_[row][col] is still true..
         */
        int r0 = std::max(0,       int(std::floor(rect.y - pos_.y)));
        int r1 = std::min(NumRows, int(std::ceil (rect.y + rect.h - pos_.y)));
        int c0 = std::max(0,       int(std::floor(rect.x - pos_.x)));
        int c1 = std::min(NumCols, int(std::ceil (rect.x + rect.w - pos_.x)));

        for (int row = r0; row < r1; ++row)
            for (int col = c0; col < c1; ++col)
                if (cells_[row][col])
                    return true;
    }

    return false;
}

void Bunker::Erode(const SDL_FRect& other)
{
    SDL_FRect rect{
        .x = pos_.x,
        .y = pos_.y,
        .w = NumCols,
        .h = NumRows,
    };
    SDL_FRect result;
    SDL_GetRectIntersectionFloat(&rect, &other, &result);

    int r0 = std::max(0,       int(std::floor(result.y - pos_.y)));
    int r1 = std::min(NumRows, int(std::ceil (result.y + result.h - pos_.y)));
    int c0 = std::max(0,       int(std::floor(result.x - pos_.x)));
    int c1 = std::min(NumCols, int(std::ceil (result.x + result.w - pos_.x)));

    for (int row = r0; row < r1; ++row)
        for (int col = c0; col < c1; ++col)
            cells_[row][col] = false;
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
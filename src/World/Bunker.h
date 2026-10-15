#ifndef BUNKER_H
#define BUNKER_H

#include <SDL3/SDL.h>

#include <array>
#include <optional>

#include "Defs.h"
#include "Util/Vector2.h"

static constexpr int NumCols = BUNKER_WIDTH;
static constexpr int NumRows = BUNKER_HEIGHT;

class Bunker
{
  public:
    Bunker()
    {
        for (auto &row : cells_)
            row.fill(true);
    }

    void SetPosition(Vector2<float> pos) { pos_ = pos; }
    void SetHealth(int health) noexcept { health_ = health; }
    [[nodiscard]] bool IsDead() const noexcept { return health_ <= 0; }
    [[nodiscard]] std::optional<SDL_FPoint> FindImpact(
        const SDL_FRect &swept, bool fromBelow = true) const;
    void Erode(const SDL_FPoint &point);
    void Carve(const SDL_FRect &rect);
    void Draw() const;

    [[nodiscard]] SDL_FRect GetRect() const
    {
        return SDL_FRect{ .x = pos_.x, .y = pos_.y, .w = NumCols, .h = NumRows };
    }

  private:
    std::array<std::array<bool, NumCols>, NumRows> cells_;
    Vector2<float> pos_;
    int health_ = 1;
};

#endif // BUNKER_H
#ifndef BUNKER_H
#define BUNKER_H

#include <SDL3/SDL.h>

#include <array>

#include "Util/Vector2.h"

static constexpr int NumCols = 64;
static constexpr int NumRows = 32;

class Bunker
{
public:
    Bunker() { for (auto& row : cells_) row.fill(true); }

    void SetPosition(Vector2<float> pos) { pos_ = pos; }
    void SetHealth(int health) noexcept { health_ = health; }
    bool IsDead() const noexcept { return health_ <= 0; }
    bool TryHit(const SDL_FRect& other);
    void Erode(const SDL_FRect& other);
    void Draw(SDL_Renderer* renderer) const;

    
private:
    
    std::array<std::array<bool, NumCols>, NumRows> cells_;
    Vector2<float> pos_;
    int health_ = 1;
};

#endif // BUNKER_H
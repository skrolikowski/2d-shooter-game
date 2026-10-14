#pragma once

#include <variant>

#include "Util/Vector2.h"
#include "World/Entity.h"

struct InvaderKilled
{
    Vector2<float> pos;
    InvaderType type;
};
struct PlayerHit
{
};
struct WaveCleared
{
};
struct ShotFired
{
    Vector2<float> pos;
    bool isPlayerFire = false;
};
// --
using Event = std::variant<InvaderKilled, PlayerHit, WaveCleared, ShotFired>;

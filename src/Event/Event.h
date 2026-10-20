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
    bool isInvaderFire = false;
};

struct BossKilled
{
    Vector2<float> pos;
    int points;
};

struct WaveCleared
{
};

struct ShotFired
{
    Vector2<float> pos;
    bool isPlayerFire = false;
};

struct PowerUpGrabbed
{
    PowerUpType powerUpType;
};

// --
using Event =
    std::variant<InvaderKilled, PlayerHit, BossKilled, WaveCleared, ShotFired, PowerUpGrabbed>;

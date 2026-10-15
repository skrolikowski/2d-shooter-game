#include "PowerUp.h"

void PowerUp::SetType(PowerUpType type)
{
    type_ = type;
    dir_ = Vector2<float>{ 0.F, 1.F }; // down to player
    speed_ = 100.F;
    width_ = 16.F;
    height_ = 16.F;

    switch (type_)
    {
        case PowerUpType::RapidFire:
            textureName_ = "pill_yellow.png";
            break;
        case PowerUpType::SpreadShot:
            textureName_ = "pill_red.png";
            break;
        case PowerUpType::Shield:
            textureName_ = "pill_blue.png";
            break;
        default:
            break;
    }
}
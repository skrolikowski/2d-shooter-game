#include "Invader.h"

void Invader::SetType(InvaderType type)
{
    type_ = type;

    switch (type_)
    {
        case InvaderType::A:
            textureName_ = "enemyBlack1.png";
            break;
        case InvaderType::B:
            textureName_ = "enemyBlack2.png";
            break;
        case InvaderType::C:
            textureName_ = "enemyBlack3.png";
            break;
        default:
            break;
    }
}
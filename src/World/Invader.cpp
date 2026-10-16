#include "Invader.h"

void Invader::SetType(InvaderType type)
{
    type_ = type;
    speed_ = 250.F;

    switch (type_)
    {
        case InvaderType::A:
            textureName_ = "enemyBlack1.png";
            break;
        case InvaderType::B:
            textureName_ = "enemyBlack2.png";
            break;
        case InvaderType::Diver:
            textureName_ = "enemyBlack3.png";
            break;
        default:
            break;
    }
}

void Invader::Update(float dt)
{
    if (type_ != InvaderType::Diver)
        return;

    switch (state_)
    {
        case InvaderState::Diving:
            diveT_ = std::min(1.F, diveT_ + (dt / INVADER_DIVE_DURATION));
            pos_ = Vector2<float>::GetCubicBezierPoint(p0_, p1_, p2_, p3_, diveT_);

            if (diveT_ >= 1.F)
            {
                returnT_ = 0.F;
                p0_ = pos_;
                p3_ = home_;
                p1_ = p0_.x < p3_.x ? Vector2<float>{ p0_.x + 64.F, p3_.y - 64.F }
                                    : Vector2<float>{ p0_.x - 64.F, p3_.y - 64.F };
                p2_ = p0_.x < p3_.x ? Vector2<float>{ p3_.x - 64.F, p3_.y - 64.F }
                                    : Vector2<float>{ p3_.x + 64.F, p3_.y - 64.F };
                state_ = InvaderState::Returning;
            }
            break;
        case InvaderState::Returning:
        {
            returnT_ = std::min(1.F, returnT_ + (dt / INVADER_RETURN_DURATION));
            pos_ = Vector2<float>::GetCubicBezierPoint(p0_, p1_, p2_, p3_, returnT_);
            if (returnT_ >= 1.F)
            {
                pos_ = home_;
                state_ = InvaderState::InFormation;
                returnT_ = 0.F;
                diveT_ = 0.F;
            }
            // Vector2<float> toHome = home_ - pos_;
            // float dist = toHome.Magnitude();
            // float step = speed_ * dt;
            // if (dist <= step)
            // {
            //     pos_ = home_;
            //     state_ = InvaderState::InFormation;
            //     diveT_ = 0.F;
            // }
            // else
            // {
            //     pos_ += toHome * (step / dist);
            // }
            break;
        }
        default:
            break;
    }
}
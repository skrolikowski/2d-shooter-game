#include <SDL3/SDL.h>
#include <iostream>

#include "Boss.h"
#include "Event/EventBus.h"
#include "Util/Texture.h"
#include "Util/Vector2.h"
#include "World.h"

#include <nlohmann/json.hpp>

Boss::Boss(const Vector2<float> &pos, const json &data) : Entity(pos)
{
    textureName_ = "enemyRed1.png";
    speed_ = 200.F;
    health_ = 8;
    healthMax_ = health_;
    width_ = 124;
    height_ = 124;

    dir_ = Vector2<float>{ 1.F, 0 };

    bulletTimerRange_ = data.at("fire_rate");
}

void Boss::updateSpawnBullet(float dt)
{
    bulletTimer_ -= dt;
    if (bulletTimer_ <= 0)
    {
        Vector2 centerPos = GetCenter();
        Vector2 bPosLeft = Vector2{ centerPos.x - (width_ / 2.F), centerPos.y + (height_ / 2.F) };
        Vector2 bPosRight = Vector2{ centerPos.x + (width_ / 2.F), centerPos.y + (height_ / 2.F) };

        bool bulletSpawnLeft = world_->SpawnEnemyBullet(bPosLeft);
        bool bulletSpawnRight = world_->SpawnEnemyBullet(bPosRight);
        if (bulletSpawnLeft || bulletSpawnRight)
        {
            ShotFired ev;
            ev.pos = bPosLeft;
            ev.isPlayerFire = false;
            EventBus::GetInstance().Push(ev);
        }

        // Reset timer..
        std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
        bulletTimer_ = delay(rng_);
    }
}

void Boss::Update(float dt)
{
    Entity::Update(dt);

    // At edge check..
    bool isAtEdge = pos_.x <= 16.F || pos_.x + width_ > SCREEN_WIDTH - 16.F;
    if (isAtEdge)
        dir_ = -dir_;

    switch (state_)
    {
        case BossState::StrafeAndAttacking:
        {
            updateSpawnBullet(dt);

            if (isAtEdge)
            {
                std::bernoulli_distribution diveChance(diveChance_);
                if (diveChance(rng_))
                {
                    SetState(BossState::Diving);

                    // control points
                    Vector2<float> p0 = GetPosition();
                    Vector2<float> p1 = Vector2<float>{
                        dir_.x < 0 ? (SCREEN_WIDTH * 0.9F) - width_ : SCREEN_WIDTH * 0.1F,
                        SCREEN_HEIGHT + (height_ * 2.5F),
                    };
                    Vector2<float> p2 = Vector2<float>{
                        dir_.x < 0 ? SCREEN_WIDTH * 0.1F : (SCREEN_WIDTH * 0.9F) - width_,
                        SCREEN_HEIGHT + (height_ * 2.5F),
                    };
                    Vector2<float> p3 = Vector2<float>{
                        dir_.x < 0 ? 32.F : SCREEN_WIDTH - 32.F - width_,
                        p0.y,
                    };
                    SetControlPoints(p0, p1, p2, p3);
                }
            }
            break;
        }
        case BossState::Diving:
        {
            diveT_ = std::min(1.F, diveT_ + (dt / BOSS_DIVE_DURATION));
            pos_ = Vector2<float>::GetCubicBezierPoint(
                controlPoints_[0], controlPoints_[1], controlPoints_[2], controlPoints_[3], diveT_);

            if (diveT_ >= 1.F)
            {
                SetState(BossState::StrafeAndAttacking);
                diveT_ = 0.F;
            }
            break;
        }
    }
}

void Boss::Draw() const
{
    Entity::Draw();

    // -- OuterBar --
    float healthBarWidth = SCREEN_WIDTH / 2.F;
    float healthBarHeight = 16.F;
    Vector2 healthBarPos = Vector2{ (SCREEN_WIDTH / 2.F) - (healthBarWidth / 2), 32.F };
    SDL_SetRenderDrawColor(Texture::Renderer, 255, 255, 255, 255);
    SDL_FRect rectInner{ healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight };
    SDL_RenderRect(Texture::Renderer, &rectInner);

    // -- InnerBar --
    float healthBarFilledWidth = float(health_) / float(healthMax_) * healthBarWidth;
    SDL_SetRenderDrawColor(Texture::Renderer, 255, 0, 0, 255);
    SDL_FRect rectOuter{ healthBarPos.x, healthBarPos.y, healthBarFilledWidth, healthBarHeight };
    SDL_RenderFillRect(Texture::Renderer, &rectOuter);
}
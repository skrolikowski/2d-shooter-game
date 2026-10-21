#include <SDL3/SDL.h>

#include "App.h"
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

        bool bulletSpawnLeft = world_->SpawnBossBullet(bPosLeft);
        bool bulletSpawnRight = world_->SpawnBossBullet(bPosRight);
        if (bulletSpawnLeft || bulletSpawnRight)
        {
            ShotFired ev;
            ev.pos = bPosLeft;
            ev.isPlayerFire = false;
            EventBus::GetInstance().Push(ev);
        }

        // Reset timer..
        std::uniform_real_distribution<float> delay(bulletTimerRange_[0], bulletTimerRange_[1]);
        bulletTimer_ = delay(App::RNG);
    }
}

void Boss::Update(float dt)
{
    Entity::Update(dt);

    switch (state_)
    {
        case BossState::StrafeAndAttacking:
        {
            // At edge check..
            bool isAtEdge = pos_.x <= 16.F || pos_.x + width_ > SCREEN_WIDTH - 16.F;
            if (isAtEdge)
                dir_ = -dir_;

            // Possibly spawn bullets..
            updateSpawnBullet(dt);

            // Possibly dive..
            if (isAtEdge)
            {
                std::bernoulli_distribution diveChance(diveChance_);
                if (diveChance(App::RNG))
                {
                    SetState(BossState::Diving);

                    bool headingLeft = dir_.x < 0;

                    // control points
                    Vector2<float> p0 = GetPosition();
                    Vector2<float> p1 = Vector2<float>{
                        headingLeft ? (SCREEN_WIDTH * 0.9F) - width_ : SCREEN_WIDTH * 0.1F,
                        SCREEN_HEIGHT,
                    };
                    Vector2<float> p2 = Vector2<float>{
                        headingLeft ? SCREEN_WIDTH * 0.1F : (SCREEN_WIDTH * 0.9F) - width_,
                        SCREEN_HEIGHT,
                    };
                    Vector2<float> p3 = Vector2<float>{
                        headingLeft ? 32.F : SCREEN_WIDTH - 32.F - width_,
                        p0.y,
                    };
                    divePath_.Start(p0, p1, p2, p3, BOSS_DIVE_DURATION);
                }
            }
            break;
        }
        case BossState::Diving:
        {
            pos_ = divePath_.Step(dt);

            if (divePath_.IsDone())
                SetState(BossState::StrafeAndAttacking);
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
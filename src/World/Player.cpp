#include "Player.h"
#include "World.h"
#include "Event/Event.h"        // ShotFired
#include "Event/EventBus.h"     // EventBus::GetInstance().Push
#include "Input/Input.h"        // Action::Left, etc..

Player::Player()
{
    textureName_ = "playerShip2_red.png";
    speed_ = 250.f;
}

void Player::HandleInput()
{
    // Direction controls..
    dir_.Reset();
    if (Input::IsDown(Action::Left))
        dir_.x -= 1;
    if (Input::IsDown(Action::Right))
        dir_.x += 1;

    // Shooting..
    if (Input::IsDown(Action::Fire) && !isShooting)
    {
        isShooting = true;
        shootTimer = 0.f;
    }
}

void Player::Update(float dt)
{
    Entity::Update(dt);

    // shoot timer / bullets..
    if (isShooting)
    {
        if (shootTimer == 0.f)
        {
            float bx = pos_.x + GetWidth() * 0.5;
            float by = pos_.y;
            Vector2 bPos = Vector2{ bx, by };
            
            if (world_->SpawnPlayerBullet(bPos))
                EventBus::GetInstance().Push(ShotFired{ bPos , true });
        }

        // --
        shootTimer += dt;
        if (shootTimer >= PLAYER_SHOOT_DT)
            isShooting = false;
    }

    // clamp to screen
    pos_.x = std::clamp(pos_.x, 0.f, SCREEN_WIDTH - GetWidth());
}


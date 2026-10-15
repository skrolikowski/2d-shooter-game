#include "Player.h"
#include "Event/Event.h"    // ShotFired
#include "Event/EventBus.h" // EventBus::GetInstance().Push
#include "Input/Input.h"    // Action::Left, etc..
#include "World.h"

Player::Player()
{
    textureName_ = "playerShip2_red.png";
    speed_ = 250.F;

    // Init PowerUp timers..
    for (auto &timer : powerUpTimers_)
        timer = 0.F;
}

bool Player::HasPowerUp(PowerUpType powerUpType) const
{
    return powerUpTimers_[static_cast<std::size_t>(powerUpType)] > 0.F;
}

void Player::GainPowerUp(PowerUpType powerUpType)
{
    powerUpTimers_[static_cast<std::size_t>(powerUpType)] = 1.F;
}

void Player::UpdatePowerUps(float dt)
{
    // Decrement PowerUp health..
    for (auto &powerUp : powerUpTimers_)
        powerUp = std::max<float>(0, powerUp - (dt * POWERUP_DECAY));
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
        shootTimer = 0.F;
    }
}

void Player::Update(float dt)
{
    Entity::Update(dt);
    UpdatePowerUps(dt);

    // shoot timer / bullets..
    if (isShooting)
    {
        if (shootTimer == 0.F)
        {
            float bx = pos_.x + (GetWidth() * 0.5F);
            float by = pos_.y;
            Vector2 bPos = Vector2{ bx, by };

            if (world_->SpawnPlayerBullet(bPos))
            {
                ShotFired ev;
                ev.pos = bPos;
                ev.isPlayerFire = true;
                EventBus::GetInstance().Push(ev);
            }
        }

        // --
        shootTimer += dt;
        if (shootTimer >= PLAYER_SHOOT_DT)
            isShooting = false;
    }

    // clamp to screen
    pos_.x = std::clamp(pos_.x, 0.F, SCREEN_WIDTH - GetWidth());
}

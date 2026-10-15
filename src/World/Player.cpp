#include "Player.h"
#include "Event/Event.h"    // ShotFired
#include "Event/EventBus.h" // EventBus::GetInstance().Push
#include "Input/Input.h"    // Action::Left, etc..
#include "World.h"

Player::Player()
{
    textureName_ = "playerShip2_red.png";
    speed_ = 250.F;
}

bool Player::HasPowerUp(PowerUpType powerUpType) const
{
    return powerUpTimers_[static_cast<std::size_t>(powerUpType)] > 0.F;
}

void Player::GainPowerUp(PowerUpType powerUpType)
{
    powerUpTimers_[static_cast<std::size_t>(powerUpType)] = POWERUP_DURATION;
}

void Player::LosePowerUp(PowerUpType powerUpType)
{
    powerUpTimers_[static_cast<std::size_t>(powerUpType)] = 0.F;
}

void Player::UpdatePowerUps(float dt)
{
    // Decrement PowerUp health..
    for (std::size_t i = 0; i < powerUpTimers_.size(); ++i)
    {
        auto type = static_cast<PowerUpType>(i);

        // Shield doesn't decay..
        if (type == PowerUpType::Shield)
            continue;

        powerUpTimers_[i] = std::max<float>(0, powerUpTimers_[i] - dt);
    }
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

        // Shot Delay (can be affected by PowerUp)
        float shotDelay =
            HasPowerUp(PowerUpType::RapidFire) ? PLAYER_RAPID_SHOOT_DELAY : PLAYER_SHOOT_DELAY;
        shootTimer += dt;
        if (shootTimer >= shotDelay)
            isShooting = false;
    }

    // clamp to screen
    pos_.x = std::clamp(pos_.x, 0.F, SCREEN_WIDTH - GetWidth());
}

void Player::Draw() const
{
    Entity::Draw();

    // Draw Shield..
    bool hasShield = HasPowerUp(PowerUpType::Shield);
    if (hasShield)
    {
        const Frame *frame = Texture::GetFrame("shield1.png");
        if (frame != nullptr)
        {
            float w = width_ * 1.5F;
            float h = height_ * 1.5F;

            // calculate position offsets based on added width..
            float ox = (w - width_) / 2.F;
            float oy = (h - height_) / 2.F;

            float sx = w / frame->rect.w;
            float sy = h / frame->rect.h;
            Texture::Draw(frame, pos_.x - ox, pos_.y - oy, sx, sy);
        }
    }
}
#include "Player.h"

Player::Player()
{
    type_ = EntityType::Player;
    speed_ = 250.f;
}

void Player::HandleInput()
{
    // Direction controls..
    dir_.Reset();
    // if (Input::IsDown(SDL_SCANCODE_W))
    //     dir_.y -= 1;
    // if (Input::IsDown(SDL_SCANCODE_S))
    //     dir_.y += 1;
    if (Input::IsDown(SDL_SCANCODE_A))
        dir_.x -= 1;
    if (Input::IsDown(SDL_SCANCODE_D))
        dir_.x += 1;

    // Shooting..
    if (Input::IsDown(SDL_SCANCODE_SPACE) && !isShooting)
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
            std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
            bullet->SetPosition(Vector2<float>{ bx, by });
            world_->Add(std::move(bullet));
        }

        // --
        shootTimer += dt;
        if (shootTimer >= PLAYER_SHOOT_DT)
            isShooting = false;
    }

    // clamp to screen
    pos_.x = std::clamp(pos_.x, 0.f, SCREEN_WIDTH - GetWidth());
}


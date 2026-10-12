#include "Player.h"
#include "World.h"

Player::Player()
{
    textureName_ = "playerShip2_red.png";
    speed_ = 250.f;
}

void Player::HandleInput()
{
    // Direction controls..
    dir_.Reset();
    if (Input::IsDown(SDL_SCANCODE_A) || Input::IsDown(SDL_SCANCODE_LEFT))
        dir_.x -= 1;
    if (Input::IsDown(SDL_SCANCODE_D) || Input::IsDown(SDL_SCANCODE_RIGHT))
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
            if (world_->SpawnPlayerBullet(Vector2{ bx, by }))
                if (onShoot)
                    onShoot();
        }

        // --
        shootTimer += dt;
        if (shootTimer >= PLAYER_SHOOT_DT)
            isShooting = false;
    }

    // clamp to screen
    pos_.x = std::clamp(pos_.x, 0.f, SCREEN_WIDTH - GetWidth());
}


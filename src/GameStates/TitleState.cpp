#include "TitleState.h"

void TitleState::Enter()
{
    titleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH1_, "2D Shooter Game", 0);
    TTF_SetTextColor(titleText_, 255, 255, 255, 255);

    subtitleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH2_, "Press Enter to Play", 0);
    TTF_SetTextColor(subtitleText_, 255, 255, 255, 255);
}

void TitleState::Exit()
{
    TTF_DestroyText(titleText_);
    TTF_DestroyText(subtitleText_);
}

void TitleState::HandleInput()
{
    if (Input::IsDown(SDL_SCANCODE_ESCAPE))
        app_->isRunning_ = false;
    else if (Input::IsDown(SDL_SCANCODE_RETURN))
        app_->ChangeState(GameStateID::Playing);
}

void TitleState::Update(float dt)
{
    // nothing..
}

void TitleState::Draw(SDL_Renderer* renderer) const
{
    // Draw Score or High Score..
    DrawText(titleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);
    DrawText(subtitleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
}
#include "GameOverState.h"

void GameOverState::Enter()
{
    app_->isPlaying_ = false;

    titleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH1_, "Game Over", 0);
    TTF_SetTextColor(titleText_, 255, 255, 255, 255);

    subtitleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH2_, "Press Enter to go to Menu", 0);
    TTF_SetTextColor(subtitleText_, 255, 255, 255, 255);
}

void GameOverState::Exit()
{
    TTF_DestroyText(titleText_);
    TTF_DestroyText(subtitleText_);
}

void GameOverState::HandleInput()
{
    if (Input::IsDown(SDL_SCANCODE_ESCAPE))
        app_->isRunning_ = false;   
    else if (Input::IsDown(SDL_SCANCODE_RETURN))
        app_->ChangeState(GameStateID::Title);
}

void GameOverState::Update(float dt)
{
    
}

void GameOverState::Draw(SDL_Renderer* renderer) const
{
    DrawText(titleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);
    DrawText(subtitleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
}
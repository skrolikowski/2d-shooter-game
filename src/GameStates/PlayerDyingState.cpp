#include "PlayerDyingState.h"

void PlayerDyingState::Enter()
{
    titleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH1_, "You're Dead!", 0);
    TTF_SetTextColor(titleText_, 255, 255, 255, 255);

    subtitleText_ = TTF_CreateText(app_->fontEngine_, app_->fontH2_, "Wait for respawn...", 0);
    TTF_SetTextColor(subtitleText_, 255, 255, 255, 255);
}

void PlayerDyingState::Exit()
{
    TTF_DestroyText(titleText_);
    TTF_DestroyText(subtitleText_);
}

void PlayerDyingState::HandleInput()
{
    
}

void PlayerDyingState::Update(float dt)
{
    stateTimer_ += dt;
    if (stateTimer_ >= stateEndTime_)
    {
        app_->ChangeState(GameStateID::Playing);
    }
    // TTF_SetTextString(subtitleText_, std::format("Wait for respawn... {:02}", startTimer_).c_str(), 0);
}

void PlayerDyingState::Draw(SDL_Renderer* renderer) const
{
    DrawText(titleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);
    DrawText(subtitleText_, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
}
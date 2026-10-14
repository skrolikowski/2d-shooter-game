#include "PlayerDyingState.h"
#include "App.h"
#include "Util/Utility.h"

void PlayerDyingState::Enter()
{
    titleText_.reset(
        TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(), "You're Dead!", 0));
    TTF_SetTextColor(titleText_.get(), 255, 255, 255, 255);

    subtitleText_.reset(
        TTF_CreateText(app_->fontEngine_.get(), app_->fontH2_.get(), "Wait for respawn...", 0));
    TTF_SetTextColor(subtitleText_.get(), 255, 255, 255, 255);
}

void PlayerDyingState::Update(float dt)
{
    stateTimer_ += dt;
    if (stateTimer_ >= stateEndTime_)
        app_->SetPendingState(GameStateID::Playing);
}

void PlayerDyingState::Draw(SDL_Renderer * /*renderer*/) const
{
    DrawText(titleText_.get(), SCREEN_WIDTH / 2.F, SCREEN_HEIGHT / 2.F, TextAlign::Center);
    DrawText(subtitleText_.get(), SCREEN_WIDTH / 2.F, (SCREEN_HEIGHT / 2.F) + 32.F,
             TextAlign::Center);
}
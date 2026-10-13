#include "App.h"
#include "TitleState.h"

void TitleState::Enter()
{
    app_->isPlaying_ = false;

    titleText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(), "2D Shooter Game", 0));
    TTF_SetTextColor(titleText_.get(), 255, 255, 255, 255);

    subtitleText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH2_.get(), "Press Enter to Play", 0));
    TTF_SetTextColor(subtitleText_.get(), 255, 255, 255, 255);
}

void TitleState::HandleInput()
{
    if (Input::IsPressed(Action::Back))
        app_->isRunning_ = false;
    else if (Input::IsPressed(Action::Confirm))
        app_->SetPendingState(GameStateID::Playing);
}

void TitleState::Draw(SDL_Renderer* renderer) const
{
    // Draw Score or High Score..
    DrawText(titleText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);
    DrawText(subtitleText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
}
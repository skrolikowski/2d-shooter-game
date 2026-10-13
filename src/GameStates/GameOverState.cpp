#include "App.h"
#include "GameOverState.h"

void GameOverState::Enter()
{
    app_->isPlaying_ = false;

    titleText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(), "Game Over", 0));
    TTF_SetTextColor(titleText_.get(), 255, 255, 255, 255);

    subtitleText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH2_.get(), "Press Enter to go to Menu", 0));
    TTF_SetTextColor(subtitleText_.get(), 255, 255, 255, 255);
}


void GameOverState::HandleInput()
{
    if (Input::IsPressed(Action::Back))
        app_->isRunning_ = false;   
    else if (Input::IsPressed(Action::Confirm))
        app_->SetPendingState(GameStateID::Title);
}

void GameOverState::Draw(SDL_Renderer* renderer) const
{
    DrawText(titleText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);
    DrawText(subtitleText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
}
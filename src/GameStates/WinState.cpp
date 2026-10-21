#include "WinState.h"
#include "App.h"
#include "Input/Input.h"
#include "Util/Utility.h"

void WinState::Enter()
{
    app_->isPlaying_ = false;

    titleText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(), "You Won!", 0));
    TTF_SetTextColor(titleText_.get(), 255, 255, 255, 255);

    subtitleText_.reset(TTF_CreateText(
        app_->fontEngine_.get(), app_->fontH2_.get(), "Press Enter to go to Menu", 0));
    TTF_SetTextColor(subtitleText_.get(), 255, 255, 255, 255);
}

void WinState::HandleInput()
{
    if (Input::IsPressed(Action::Confirm))
        app_->SetPendingState(GameStateID::Title);
}

void WinState::Draw(SDL_Renderer * /*renderer*/) const
{
    DrawText(titleText_.get(), SCREEN_WIDTH / 2.F, SCREEN_HEIGHT / 2.F, TextAlign::Center);
    DrawText(
        subtitleText_.get(), SCREEN_WIDTH / 2.F, (SCREEN_HEIGHT / 2.F) + 32.F, TextAlign::Center);
}
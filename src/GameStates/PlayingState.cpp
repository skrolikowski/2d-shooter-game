#include "App.h"
#include "PlayingState.h"

void PlayingState::Enter()
{
    if (app_->isPlaying_)
        resetPlayerPosition();
    else
        startNewGame();
}

void PlayingState::HandleInput()
{
    if (Input::IsPressed(SDL_SCANCODE_ESCAPE))
        app_->SetPendingState(GameStateID::Title);
}


void PlayingState::startNewGame()
{
    app_->world_->Clear();
    StartNewWave();

    // --
    app_->isPlaying_ = true;
}

void PlayingState::StartNewWave()
{
    // --
    const json& waveData = app_->GetLevelData();    
    app_->world_->GetFormation()->CreateWave(waveData, *app_->textureManager_);

    // --
    // Add Bunkers..
    app_->world_->SpawnBunker(Vector2{ 128.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });
    app_->world_->SpawnBunker(Vector2{ SCREEN_WIDTH / 2.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });
    app_->world_->SpawnBunker(Vector2{ SCREEN_WIDTH - 128.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });

    app_->ResetPlayerStats();
}

void PlayingState::resetPlayerPosition()
{
    app_->world_->SetPlayerPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 80.f });
}
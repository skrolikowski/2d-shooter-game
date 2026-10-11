#include "App.h"
#include "PlayingState.h"

void PlayingState::Enter()
{
    if (app_->isPlaying_)
        resetPlayerPosition();
    else
        startNewGame();
}

void PlayingState::Update(float dt)
{
    waveTextTimer_ += dt;
    if (waveTextTimer_ >= waveTextInterval_)
    {
        levelText_.release();
        waveText_.release();
    }
}

void PlayingState::HandleInput()
{
    if (Input::IsPressed(SDL_SCANCODE_ESCAPE))
        app_->SetPendingState(GameStateID::Title);
}

void PlayingState::Draw(SDL_Renderer* renderer) const
{
    if (levelText_)
        DrawText(levelText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, TextAlign::Center);

    if (waveText_)
        DrawText(waveText_.get(), SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 32.f, TextAlign::Center);
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
    // Wave notification..
    levelText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(), std::format("Level {:01}", (app_->level_ + 1)).c_str(), 0));
    waveText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH2_.get(), std::format("Wave {:01}", (app_->levelWave_ + 1)).c_str(), 0));
    TTF_SetTextColor(levelText_.get(), 255, 255, 255, 255);
    TTF_SetTextColor(waveText_.get(), 255, 255, 255, 255);
    waveTextTimer_ = 0.f;
    
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
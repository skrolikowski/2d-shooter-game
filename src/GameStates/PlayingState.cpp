#include "PlayingState.h"
#include "App.h"
#include "Input/Input.h"
#include "Util/Utility.h"

void PlayingState::Enter()
{
    if (app_->isPlaying_)
    {
        waveStarted_ = true;
        respawnPlayer();
    }
    else
        StartNewWave();
}

void PlayingState::Update(float dt)
{
    waveTextTimer_ = std::min(waveTextInterval_, waveTextTimer_ + dt);

    if (!waveStarted_ && waveTextTimer_ >= waveTextInterval_)
    {
        levelText_.reset();
        waveText_.reset();

        waveStarted_ = true;
        spawnWaveEntities();
    }
}

void PlayingState::HandleInput()
{
    if (Input::IsPressed(Action::Back))
        app_->SetPendingState(GameStateID::Title);
}

void PlayingState::Draw(SDL_Renderer * /*renderer*/) const
{
    if (levelText_)
        DrawText(levelText_.get(), SCREEN_WIDTH / 2.F, SCREEN_HEIGHT / 2.F, TextAlign::Center);

    if (waveText_)
        DrawText(
            waveText_.get(), SCREEN_WIDTH / 2.F, (SCREEN_HEIGHT / 2.F) + 32.F, TextAlign::Center);
}

void PlayingState::StartNewWave()
{
    app_->world_->Clear();
    waveStarted_ = false;
    app_->isPlaying_ = true;

    // Wave notification..
    levelText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH1_.get(),
        std::format("Level {:01}", (app_->level_ + 1)).c_str(), 0));
    waveText_.reset(TTF_CreateText(app_->fontEngine_.get(), app_->fontH2_.get(),
        std::format("Wave {:01}", (app_->levelWave_ + 1)).c_str(), 0));
    TTF_SetTextColor(levelText_.get(), 255, 255, 255, 255);
    TTF_SetTextColor(waveText_.get(), 255, 255, 255, 255);
    waveTextTimer_ = 0.F;
    waveStarted_ = false;
}

void PlayingState::spawnWaveEntities()
{
    // Create Player.. (if necessary)
    app_->world_->SpawnPlayer(Vector2<float>{ SCREEN_WIDTH / 2.F, SCREEN_HEIGHT - 96.F });

    // Add Wave..
    const json &waveData = app_->GetLevelData();
    std::string waveType = waveData.at("type").get<std::string>();
    // ..wave type..
    if (waveType == "invaders")
        app_->world_->GetFormation()->CreateWave(waveData);
    else if (waveType == "boss")
        app_->world_->SpawnBoss(waveData);

    // --
    // Add Bunkers..
    app_->world_->SpawnBunker(Vector2{ 128.F - (NumCols / 2.F), SCREEN_HEIGHT - 150.F });
    app_->world_->SpawnBunker(
        Vector2{ (SCREEN_WIDTH / 2.F) - (NumCols / 2.F), SCREEN_HEIGHT - 150.F });
    app_->world_->SpawnBunker(
        Vector2{ SCREEN_WIDTH - 128.F - (NumCols / 2.F), SCREEN_HEIGHT - 150.F });
}

void PlayingState::respawnPlayer()
{
    app_->world_->SpawnPlayer(Vector2<float>{ SCREEN_WIDTH / 2.F, SCREEN_HEIGHT - 80.F });
}
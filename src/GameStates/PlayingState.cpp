#include "App.h"
#include "PlayingState.h"

void PlayingState::Enter()
{
    if (!app_->isPlaying_)
        startNewGame();
    else
        resetPlayerPosition();
}

void PlayingState::HandleInput()
{
    if (Input::IsPressed(SDL_SCANCODE_ESCAPE))
        app_->SetPendingState(GameStateID::Title);
}

void PlayingState::startNewGame()
{
    app_->ResetPlayerStats();
    app_->world_->Clear();

    // --
    // Add Player..
    std::unique_ptr<Player> player = std::make_unique<Player>();
    player->SetPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 80.f });
    std::shared_ptr<Texture> playerTexture = app_->textureManager_->Get("res/player_red.png");
    player->SetTexture(playerTexture);
    
    // Add Formation / enemies..
    std::unique_ptr<Formation> formation = std::make_unique<Formation>(*app_->textureManager_);
    
    // Add Bunkers..
    app_->world_->SpawnBunker(Vector2{ 128.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });
    app_->world_->SpawnBunker(Vector2{ SCREEN_WIDTH / 2.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });
    app_->world_->SpawnBunker(Vector2{ SCREEN_WIDTH - 128.f - NumCols / 2.f, SCREEN_HEIGHT - 150.f });

    // Add to world..
    app_->world_->AddPlayer(std::move(player));
    app_->world_->AddFormation(std::move(formation));

    app_->isPlaying_ = true;
}

void PlayingState::resetPlayerPosition()
{
    app_->world_->SetPlayerPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 80.f });
}
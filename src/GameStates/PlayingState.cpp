#include "PlayingState.h"

void PlayingState::Enter()
{
    if (!app_->isPlaying_)
        startNewGame();
    else
        resetPlayerPosition();
}

void PlayingState::Exit()
{

}

void PlayingState::HandleInput()
{
    if (Input::IsDown(SDL_SCANCODE_ESCAPE))
        app_->isRunning_ = false;   
}


void PlayingState::Update(float dt)
{
    
}

void PlayingState::Draw(SDL_Renderer* renderer) const
{
    
}

void PlayingState::startNewGame()
{
    // Add Player..
    std::unique_ptr<Player> player = std::make_unique<Player>();
    player->SetPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 32.f });
    std::shared_ptr<Texture> playerTexture = app_->textureManager_->Get("res/player_red.png");
    player->SetTexture(playerTexture);
    
    // reset game data..
    app_->playerScore_ = 0;
    app_->playerLives_ = app_->playerInitLives_;
    
    // Add Formation / enemies..
    std::unique_ptr<Formation> formation = std::make_unique<Formation>(*app_->textureManager_);
    
    // Add to world..
    app_->world_->Add(std::move(player));
    app_->world_->AddFormation(std::move(formation));

    app_->isPlaying_ = true;
}

void PlayingState::resetPlayerPosition()
{
    // player->SetPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 32.f);
}
#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Defs.h"
#include "GameStates/GameState.h"
#include "Util/Input.h"
#include "Util/Utility.h"
#include "Util/Texture.h"
#include "Util/TextureManager.h"
#include "World/World.h"
#include "World/Player.h"

#include <iostream>
#include <cstdio>
#include <string>

struct WindowParams
{
    std::string title;
    int width;
    int height;
    bool isResizeable = true;
    bool isFullscreen = false;
};

class App
{
    friend class TitleState;
    friend class PlayingState;
    friend class PlayerDyingState;
    friend class GameOverState;

public:
    App() = default;
    ~App() { Quit(); }

    void Init(const WindowParams& params);
    void MainLoop();

    void HandleInput();
    void Update(float dt);
    void Draw();
    void Quit();

    void RefreshHud();
    void ResetPlayerStats();
    void SetPendingState(GameStateID stateID) { pendingStateID_ = stateID; }

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool isRunning_ = false;
    bool isPlaying_ = false;

    TTF_Text* playerScoreText_ = nullptr;
    TTF_Text* playerLivesText_ = nullptr;
    int playerScore_ = 0;
    int playerLives_ = 0;
    int playerInitLives_ = 3;

    GameStateID stateID_ = GameStateID::Title;
    std::optional<GameStateID> pendingStateID_;
    std::unique_ptr<GameState> state_ = nullptr;
    
    TTF_Font* fontH1_ = nullptr;
    TTF_Font* fontH2_ = nullptr;
    TTF_Font* fontH3_ = nullptr;
    TTF_TextEngine* fontEngine_ = nullptr;

    std::unique_ptr<TextureManager> textureManager_ = nullptr;
    std::unique_ptr<World> world_ = nullptr;

    void handlePlayerHit();
    void handleInvaderKill();
    void changeState(GameStateID stateID);
  
};

#endif // APP_H

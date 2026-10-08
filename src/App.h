#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Defs.h"
#include "Utility.h"
#include "GameStates/GameState.h"
#include "GameStates/TitleState.h"
#include "GameStates/PlayingState.h"
#include "GameStates/PlayerDyingState.h"
#include "GameStates/GameOverState.h"
#include "TextureManager.h"
#include "Texture.h"
#include "World.h"
#include "Input.h"
#include "Player.h"

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

    void ChangeState(GameStateID stateID);

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool isRunning_ = false;
    bool isPlaying_ = false;

    TTF_Text* playerScoreText_;
    TTF_Text* playerLivesText_;
    int playerScore_;
    int playerLives_;
    int playerInitLives_ = 3;

    GameStateID stateID_;
    std::unique_ptr<GameState> state_ = nullptr;
    
    TTF_Font* fontH1_;
    TTF_Font* fontH2_;
    TTF_Font* fontH3_;
    TTF_TextEngine* fontEngine_;

    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<World> world_;

    void handlePlayerHit();
    void handleInvaderKill();
};

#endif // APP_H

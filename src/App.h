#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Defs.h"
#include "Formation.h"
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

enum class State { Title, Playing, PlayerDying, GameOver };

class App
{
public:
    App() = default;
    ~App() { Quit();}

    void Init(const WindowParams& params);
    void MainLoop();
    void Start();
    void HandleHit();
    void HandleKill();
    void HandleInput();
    void Update(float dt);
    void Draw();
    void Quit();

    void onKeyDown(SDL_KeyboardEvent e);

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool isRunning_ = false;

    State state_;
    int playerScore_;
    int playerLives_;
    int playerInitLives_ = 3;
    
    TTF_Font* font_;
    TTF_TextEngine* fontEngine_;
    TTF_Text* scoreText_;

    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<World> world_;
};

#endif // APP_H

#include "App.h"

void App::Init(const WindowParams& params)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return;
    }

    // Create window..
    SDL_PropertiesID winProps = SDL_CreateProperties();
    SDL_SetStringProperty(winProps, SDL_PROP_WINDOW_CREATE_TITLE_STRING, params.title.c_str());
    SDL_SetNumberProperty(winProps, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, params.width);
    SDL_SetNumberProperty(winProps, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, params.height);
    SDL_SetBooleanProperty(winProps, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, params.isResizeable);
    SDL_SetBooleanProperty(winProps, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, params.isFullscreen);
    window_ = SDL_CreateWindowWithProperties(winProps);
    SDL_DestroyProperties(winProps);
    if (!window_)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    // Create renderer..
    SDL_PropertiesID renderProps = SDL_CreateProperties();
    SDL_SetPointerProperty(renderProps, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window_);
    renderer_ = SDL_CreateRendererWithProperties(renderProps);
    SDL_DestroyProperties(renderProps);
    if (!renderer_)
    {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return;
    }

    TTF_Init();
    font_ = TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 8.0f);   // size in points
    fontEngine_ = TTF_CreateRendererTextEngine(renderer_);
    scoreText_ = TTF_CreateText(fontEngine_, font_, "SCORE 0000", 0);
    TTF_SetTextColor(scoreText_, 255, 255, 255, 255);

    Start();
}

void App::Start()
{
    // TextureManager..
    textureManager_ = std::make_unique<TextureManager>();
    // Texture..
    Texture::SetDefaultRenderer(renderer_);
    
    // Add Player..
    std::unique_ptr<Player> player = std::make_unique<Player>();
    player->SetPosition(Vector2<float>{ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 64.f });
    std::shared_ptr<Texture> playerTexture = textureManager_->Get("res/player_red.png");
    player->SetTexture(playerTexture);

    // Add Formation / enemies..
    std::unique_ptr<Formation> formation = std::make_unique<Formation>(*textureManager_);
    
    // World..
    world_ = std::make_unique<World>();
    world_->Add(std::move(player));
    world_->AddFormation(std::move(formation));
    // ..callbacks..
    world_->onPlayerHit   = [this]{ HandleHit(); };
    world_->onEnemyKilled = [this]{ HandleKill(); };

    playerScore_ = 0;
    playerLives_ = playerInitLives_;

    MainLoop();
}

void App::MainLoop()
{
    Input::Init();

    Uint64 prevTick = SDL_GetTicksNS();
    double accumulator = 0.0;
    isRunning_ = true;

    while (isRunning_)
    {
        Input::Update();
        HandleInput();

        // dt
        Uint64 nextTick = SDL_GetTicksNS();
        double frame = (nextTick - prevTick) / 1e9;
        // float dt = static_cast<float>(nextTick - prevTick) / 1000.0f;
        prevTick = nextTick;
        if (frame > 0.25) frame = 0.25;
        accumulator += frame;

        while (accumulator >= FRAME_STEP)
        {
            Update(static_cast<float>(FRAME_STEP));
            accumulator -= FRAME_STEP;
        }

        Draw();
    }
}

void App::HandleHit()
{
    playerLives_ -= 1;
    if (playerLives_ == 0)
    {
        //TODO: game over
    }
}

void App::HandleKill()
{
    playerScore_ += 1;
    TTF_SetTextString(scoreText_, std::format("SCORE {:04}", playerScore_).c_str(), 0);
}

void App::HandleInput()
{
    world_->HandleInput();

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_EVENT_QUIT:
                isRunning_ = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch(e.key.key)
                {
                    case SDLK_ESCAPE:
                        isRunning_ = false;
                        break;
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                // SDL_GetWindowSize(window, &camera.mWidth, &camera.mHeight);
                break;
        }
    }
}

void App::Update(float dt)
{
    world_->Update(dt);
}

void App::Draw()
{
    SDL_SetRenderDrawColor(renderer_, 25, 15, 16, 0);
    SDL_RenderClear(renderer_);

    TTF_DrawRendererText(scoreText_, 16.f, 16.f);

    world_->Draw(renderer_);

    SDL_RenderPresent(renderer_);
}

void App::Quit()
{
    TTF_DestroyText(scoreText_);
    TTF_DestroyRendererTextEngine(fontEngine_);
    TTF_CloseFont(font_);
    TTF_Quit();

    textureManager_->Destroy();

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
#include "App.h"
#include "GameStates/TitleState.h"
#include "GameStates/PlayingState.h"
#include "GameStates/PlayerDyingState.h"
#include "GameStates/GameOverState.h"

void App::Init(const WindowParams& params)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
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
    window_.reset(SDL_CreateWindowWithProperties(winProps));
    SDL_DestroyProperties(winProps);
    if (!window_)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    // Create renderer..
    SDL_PropertiesID renderProps = SDL_CreateProperties();
    SDL_SetPointerProperty(renderProps, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window_.get());
    renderer_.reset(SDL_CreateRendererWithProperties(renderProps));
    SDL_DestroyProperties(renderProps);
    if (!renderer_)
    {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return;
    }

    // TextureManager..
    textureManager_ = std::make_unique<TextureManager>();
    // Texture (set default renderer)..
    Texture::SetDefaultRenderer(renderer_.get());

    // Set Font..
    TTF_Init();
    fontH1_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 24.0f));
    fontH2_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 16.0f));
    fontH3_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 8.0f));
    fontEngine_.reset(TTF_CreateRendererTextEngine(renderer_.get()));
    
    // --
    playerScoreText_.reset(TTF_CreateText(fontEngine_.get(), fontH3_.get(), "SCORE 0000", 0));
    TTF_SetTextColor(playerScoreText_.get(), 255, 255, 255, 255);
    playerLivesText_.reset(TTF_CreateText(fontEngine_.get(), fontH3_.get(), "LIVES 3x", 0));
    TTF_SetTextColor(playerLivesText_.get(), 255, 255, 255, 255);

    // Audio..
    SDL_AudioSpec spec;
    Uint8* buf = nullptr;
    if (!SDL_LoadWAV("res/audio/laserShoot.wav", &spec, &buf, &wavLength_))
    {
        SDL_Log("SDL_LoadWAV failed: %s", SDL_GetError());
        return;
    }
    wavBuffer_.reset(buf);

    audioStream_.reset(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL));
    if (!audioStream_)
    {
        SDL_Log("SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        return;
    }

    // SDL_PutAudioStreamData(audioStream_, buf, len);
    SDL_ResumeAudioStreamDevice(audioStream_.get());

    // Create world..
    world_ = std::make_unique<World>();
    // callbacks
    world_->onPlayerShoot = [this]{ handleBulletShoot(); };
    world_->onInvaderShoot = [this]{ handleBulletShoot(); };
    world_->onPlayerHit   = [this]{ handlePlayerHit(); };
    world_->onEnemyKilled = [this]{ handleInvaderKill(); };

    // --
    changeState(GameStateID::Title);
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

void App::HandleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_EVENT_QUIT:
                isRunning_ = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                // SDL_GetWindowSize(window, &camera.mWidth, &camera.mHeight);
                break;
        }
    }

    // Handle World input..
    switch (stateID_)
    {
        case GameStateID::Playing:
            world_->HandleInput();
            break;
        default:
            break;
    }

    // Handle State input..
    if (state_ != nullptr)
        state_->HandleInput();
}

void App::Update(float dt)
{
    // Handle World update..
    switch (stateID_)
    {
        case GameStateID::Playing:
            world_->Update(dt);
            break;
        default:
            break;
    }

    // Handle State update..
    if (state_ != nullptr)
        state_->Update(dt);

    // Handle requested state change..
    if (pendingStateID_)
    {
        changeState(*pendingStateID_);
        pendingStateID_.reset();
    }
}

void App::Draw()
{
    SDL_SetRenderDrawColor(renderer_.get(), 25, 15, 16, 0);
    SDL_RenderClear(renderer_.get());
    // --

    // Draw Score or High Score..
    DrawText(playerScoreText_.get(), 16.f, 16.f, TextAlign::Left);
    DrawText(playerLivesText_.get(), SCREEN_WIDTH - 16.f, 16.f, TextAlign::Right);

    // Handle World draw..
    switch (stateID_)
    {
        case GameStateID::Playing:
        case GameStateID::PlayerDying:
            world_->Draw(renderer_.get());
            break;
        default:
            break;
    }

    // Handle State draw..
    if (state_ != nullptr)
        state_->Draw(renderer_.get());

    // --
    SDL_RenderPresent(renderer_.get());
}

void App::Quit()
{
    if (world_ != nullptr)
        world_->Clear();

    if (state_ != nullptr)
    {
        state_->Exit();
        state_.reset(); // release ownership;
    }

    // Destroy audio..
    audioStream_.reset();
    wavBuffer_.reset();

    if (textureManager_ != nullptr)
    {
        textureManager_->Destroy();
        textureManager_.reset(); // release ownership;
    }

    // Destroy TTF
    playerScoreText_.reset();
    playerLivesText_.reset();
    fontEngine_.reset();
    fontH1_.reset();
    fontH2_.reset();
    fontH3_.reset();
    TTF_Quit();

    renderer_.reset();
    window_.reset();
    SDL_Quit();
}

void App::changeState(GameStateID stateID)
{
    if (state_ != nullptr)
    {
        state_->Exit();
        state_ = nullptr;
    }

    switch(stateID)
    {
        case GameStateID::Title:
            state_ = std::make_unique<TitleState>(this);
            break;
        case GameStateID::Playing:
            state_ = std::make_unique<PlayingState>(this);
            break;
        case GameStateID::PlayerDying:
            state_ = std::make_unique<PlayerDyingState>(this);
            break;
        case GameStateID::GameOver:
            state_ = std::make_unique<GameOverState>(this);
            break;
    }

    state_->Enter();
    stateID_ = stateID;
}

void App::ResetPlayerStats()
{
    playerLives_ = playerInitLives_;
    playerScore_ = 0;
    RefreshHud();
}

void App::RefreshHud()
{
    TTF_SetTextString(playerLivesText_.get(), std::format("LIVES {:01}x", playerLives_).c_str(), 0);
    TTF_SetTextString(playerScoreText_.get(), std::format("SCORE {:04}", playerScore_).c_str(), 0);
}

void App::handlePlayerHit()
{
    playerLives_ -= 1;
    RefreshHud();
    
    if (playerLives_ == 0)
        SetPendingState(GameStateID::GameOver);
    else
        SetPendingState(GameStateID::PlayerDying);
}

void App::handleInvaderKill()
{
    playerScore_ += 1;
    RefreshHud();
}

void App::handleBulletShoot()
{
    SDL_PutAudioStreamData(audioStream_.get(), wavBuffer_.get(), static_cast<int>(wavLength_));
}
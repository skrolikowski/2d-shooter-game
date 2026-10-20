#include <algorithm>
#include <fstream>

#include "App.h"
#include "GameStates/GameOverState.h"
#include "GameStates/PlayerDyingState.h"
#include "GameStates/PlayingState.h"
#include "GameStates/TitleState.h"
#include "Input/Input.h"
#include "Util/Audio.h"
#include "Util/Texture.h"
#include "Util/Utility.h"

void App::Init(const WindowParams &params)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
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
    SDL_SetBooleanProperty(
        winProps, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, params.isFullscreen);
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
    // Texture (set default renderer)..
    Texture::SetDefaultRenderer(renderer_.get());
    Texture::LoadSpritesheet("res/sprites/entities.json");

    // Set Font..
    TTF_Init();
    fontH1_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 24.0F));
    fontH2_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 16.0F));
    fontH3_.reset(TTF_OpenFont("res/font/PressStart2P-Regular.ttf", 12.0F));
    fontEngine_.reset(TTF_CreateRendererTextEngine(renderer_.get()));

    // --
    playerScoreText_.reset(TTF_CreateText(fontEngine_.get(), fontH3_.get(), "SCORE 0000", 0));
    TTF_SetTextColor(playerScoreText_.get(), 255, 255, 255, 255);

    // Load Audio Library..
    if (Audio::Init())
    {
        Audio::Load("laserShoot", "res/audio/laserShoot.wav");
        Audio::Load("explosion", "res/audio/explosion.wav");
        Audio::Load("powerUp", "res/audio/powerUp.wav");
    }

    // Create World..
    world_ = std::make_unique<World>();

    // Create Camera..
    camera_ = std::make_unique<Camera>();

    // Subscribe to Event Bus..
    Listen<InvaderKilled>([this](const InvaderKilled &e) { handleInvaderKilled(e.pos, e.type); });
    Listen<PlayerHit>([this](const PlayerHit &e) { handlePlayerHit(e.isInvaderFire); });
    Listen<BossKilled>([this](const BossKilled &e) { handleBossKilled(e.pos, e.points); });
    Listen<WaveCleared>([this](const WaveCleared &) { handleFormationKill(); });
    Listen<ShotFired>([this](const ShotFired &e) { handleBulletShoot(e.pos, e.isPlayerFire); });
    Listen<PowerUpGrabbed>(
        [this](const PowerUpGrabbed &e) { handlePowerUpGrabbed(e.powerUpType); });

    // --
    // Load level data..
    std::ifstream file("res/levels.json");
    if (!file)
    {
        SDL_Log("Could not open res/levels.json");
        Quit();
        return;
    }

    try
    {
        levelData_ = json::parse(file).at("levels");
    }
    catch (const json::exception &e)
    {
        SDL_Log("levels.json: %s", e.what());
    }

    // --
    changeState(GameStateID::Title);
    MainLoop();
}

void App::MainLoop()
{
    Uint64 prevTick = SDL_GetTicksNS();
    double accumulator = 0.0;
    isRunning_ = true;

    while (isRunning_)
    {
        HandleInput();

        // dt
        Uint64 nextTick = SDL_GetTicksNS();
        double frame = static_cast<double>(nextTick - prevTick) / 1e9;
        prevTick = nextTick;
        frame = std::min(frame, 0.25); // avoid "spiral of death"
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
        switch (e.type)
        {
            case SDL_EVENT_GAMEPAD_ADDED:
                // Called on SDL_Init() or when Gamepad added..
                Input::OnGamepadAdded(e.gdevice.which);
                break;
            case SDL_EVENT_GAMEPAD_REMOVED:
                Input::OnGamepadRemoved(e.gdevice.which);
                break;
            case SDL_EVENT_QUIT:
                isRunning_ = false;
                break;
            // case SDL_EVENT_WINDOW_RESIZED:
            //     SDL_GetWindowSize(window, &camera.mWidth, &camera.mHeight);
            //     break;
            default:
                break;
        }
    }

    // Poll input..
    Input::Update();

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

    camera_->Update(dt);

    // Handle World update..
    switch (stateID_)
    {
        case GameStateID::Playing:
            world_->Update(dt);
            break;
        default:
            break;
    }

    // Handle Event Bus..
    EventBus::GetInstance().Dispatch();

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

void App::Draw() const
{
    SDL_SetRenderDrawColor(renderer_.get(), 25, 15, 16, 0);
    SDL_RenderClear(renderer_.get());
    // --

    // Set Render Viewport (Camera Shake)
    SDL_FRect rect{ Camera::Offset.x, Camera::Offset.y, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_SetRenderViewportFloat(renderer_.get(), &rect);

    // Handle World draw..
    switch (stateID_)
    {
        case GameStateID::Playing:
        case GameStateID::PlayerDying:
            world_->Draw();
            break;
        default:
            break;
    }

    // Handle State draw..
    if (state_ != nullptr)
        state_->Draw(renderer_.get());

    DrawUI();

    // --
    SDL_RenderPresent(renderer_.get());
}

void App::DrawUI() const
{
    // Reset Render Viewport
    SDL_FRect origRect{ 0.F, 0.F, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_SetRenderViewportFloat(renderer_.get(), &origRect);

    // Draw Score or High Score..
    DrawText(playerScoreText_.get(), SCREEN_WIDTH / 2.F, 16.F, TextAlign::Center);

    // Draw Player live count display..
    int x1 = SCREEN_WIDTH - 96;
    float y1 = 16.F;
    int padX = 24;
    float w = 18.F;
    float h = 18.F;

    for (int i = 0, j = x1; i < playerInitLives_; ++i)
    {
        if (i < playerLives_)
        {
            const Frame *frame =
                Texture::GetFrame(i < playerLives_ ? "star_gold.png" : "star_silver.png");
            if (frame != nullptr)
            {
                auto x = static_cast<float>(j);
                float sx = w / frame->rect.w;
                float sy = h / frame->rect.h;
                Texture::Draw(frame, x, y1, sx, sy);
            }
        }
        j += padX;
    }

    // --
    // -- PowerUp Display
    int x2 = 16;
    float y2 = 16.F;

    bool hasRapidFire = world_->PlayerHasPowerUp(PowerUpType::RapidFire);
    if (hasRapidFire)
    {
        const Frame *f = Texture::GetFrame("pill_red.png");
        if (f != nullptr)
        {
            auto x = static_cast<float>(x2);
            float sx = w / f->rect.w;
            float sy = h / f->rect.h;
            Texture::Draw(f, x, y2, sx, sy);
        }
        x2 += padX;
    }
    bool hasSpreadShot = world_->PlayerHasPowerUp(PowerUpType::SpreadShot);
    if (hasSpreadShot)
    {
        const Frame *f = Texture::GetFrame("pill_yellow.png");
        if (f != nullptr)
        {
            auto x = static_cast<float>(x2);
            float sx = w / f->rect.w;
            float sy = h / f->rect.h;
            Texture::Draw(f, x, y2, sx, sy);
        }
        x2 += padX;
    }
    bool hasShield = world_->PlayerHasPowerUp(PowerUpType::Shield);
    if (hasShield)
    {
        const Frame *f = Texture::GetFrame("pill_blue.png");
        if (f != nullptr)
        {
            auto x = static_cast<float>(x2);
            float sx = w / f->rect.w;
            float sy = h / f->rect.h;
            Texture::Draw(f, x, y2, sx, sy);
        }
    }
}

void App::Quit()
{
    if (world_ != nullptr)
    {
        world_->Clear();
        world_.reset();
    }

    if (state_ != nullptr)
    {
        state_->Exit();
        state_.reset(); // release ownership;
    }

    // Destroy audio..
    Audio::Shutdown();

    // Unload loaded textures..
    Texture::UnloadFile();

    // Destroy TTF
    playerScoreText_.reset();
    fontEngine_.reset();
    fontH1_.reset();
    fontH2_.reset();
    fontH3_.reset();
    TTF_Quit();

    renderer_.reset();
    window_.reset();
    SDL_Quit();
}

unsigned long App::GetNumLevels() const
{
    return levelData_.size();
}

unsigned long App::GetNumWaves() const
{
    return levelData_.at(level_).at("waves").size();
}

const json &App::GetLevelData() const
{
    return levelData_.at(level_).at("waves").at(levelWave_);
}

void App::changeState(GameStateID stateID)
{
    if (state_ != nullptr)
    {
        state_->Exit();
        state_ = nullptr;
    }

    switch (stateID)
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
    TTF_SetTextString(playerScoreText_.get(), std::format("SCORE {:04}", playerScore_).c_str(), 0);
}

void App::handlePlayerHit(bool isInvaderFire)
{
    bool playerHasShield = world_->PlayerHasPowerUp(PowerUpType::Shield);
    if (playerHasShield)
        world_->RemovePlayerPowerUp(PowerUpType::Shield);

    if (isInvaderFire && playerHasShield)
        return;

    // Player takes damage..
    world_->DamagePlayer(1);
    playerLives_ -= 1;

    // if (playerLives_ == 0)
    //     SetPendingState(GameStateID::GameOver);
    // else
    //     SetPendingState(GameStateID::PlayerDying);
}

void App::handleBossKilled(Vector2<float> pos, int points)
{
    // Explosion!!
    Audio::Play("explosion");
    world_->SpawnExplosion(pos);

    // Update score..
    playerScore_ += points;
    RefreshHud();
}

void App::handleInvaderKilled(Vector2<float> pos, InvaderType /*type*/)
{
    // Explosion!!
    Audio::Play("explosion");
    world_->SpawnExplosion(pos);

    // PowerUp (% spawn chance)..
    std::bernoulli_distribution _spawnChance(POWERUP_SPAWN_CHANCE);
    if (_spawnChance(rng_))
    {
        int spawnTypeCount = static_cast<int>(PowerUpType::Count);
        std::uniform_int_distribution<int> _spawnType(0, spawnTypeCount - 1);
        auto spawnType = _spawnType(rng_);

        world_->SpawnPowerUp(pos, static_cast<PowerUpType>(spawnType));
    }

    // Update score..
    playerScore_ += 1;
    RefreshHud();
}

void App::handleBulletShoot(Vector2<float> /*pos*/, bool /*isPlayerFire*/)
{
    Audio::Play("laserShoot");
}

void App::handleFormationKill()
{
    // Check for new Wave..
    if (levelWave_ + 1 < GetNumWaves())
    {
        // Go to next wave..
        ++levelWave_;
        if (stateID_ == GameStateID::Playing)
        {
            auto *state = dynamic_cast<PlayingState *>(state_.get());
            if (state != nullptr)
                state->StartNewWave();
        }
        else
            changeState(GameStateID::Playing);
    }

    // Check for new Level..
    else if (level_ + 1 < GetNumLevels())
    {
        ++level_;
        levelWave_ = 0;
    }
    else
    {
        // changeState(GameStateID::Win);
    }
}

void App::handlePowerUpGrabbed(PowerUpType powerUpType)
{
    Audio::Play("powerUp");

    world_->GivePlayerPowerUp(powerUpType);
}
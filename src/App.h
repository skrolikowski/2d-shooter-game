#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Event/EventListener.h"
#include "GameStates/GameState.h"
#include "Util/Camera.h"
#include "Util/SDLPtr.h"
#include "World/World.h"

#include <cassert>
#include <memory>
#include <optional>
#include <string>

struct WindowParams
{
    std::string title;
    int width;
    int height;
    bool isResizeable = true;
    bool isFullscreen = false;
};

class App : public EventListener
{
    friend class TitleState;
    friend class PlayingState;
    friend class PlayerDyingState;
    friend class GameOverState;

  public:
    App() = default;
    ~App() { Quit(); }

    void Init(const WindowParams &params);
    void MainLoop();

    [[nodiscard]] const json &GetLevelData() const;
    [[nodiscard]] unsigned long GetNumLevels() const;
    [[nodiscard]] unsigned long GetNumWaves() const;

    void HandleInput();
    void Update(float dt);
    void Draw();
    void Quit();

    void RefreshHud();
    void ResetPlayerStats();
    void SetPendingState(GameStateID stateID) { pendingStateID_ = stateID; }

  private:
    WindowPtr window_;
    RendererPtr renderer_;
    bool isRunning_ = false;
    bool isPlaying_ = false;

    TTF_TextPtr playerScoreText_;
    TTF_TextPtr playerLivesText_;
    int playerScore_ = 0;
    int playerLives_ = 0;
    int playerInitLives_ = 3;

    unsigned long level_ = 0;
    unsigned long levelWave_ = 0;
    json levelData_ = json::array();

    GameStateID stateID_ = GameStateID::Title;
    std::optional<GameStateID> pendingStateID_;
    std::unique_ptr<GameState> state_;

    TTF_FontPtr fontH1_;
    TTF_FontPtr fontH2_;
    TTF_FontPtr fontH3_;
    TTF_TextEnginePtr fontEngine_;

    std::unique_ptr<Camera> camera_;
    std::unique_ptr<World> world_;

    void handlePlayerHit();
    void handleBulletShoot(Vector2<float> pos, bool isPlayerFire = false);
    void handleInvaderKilled(Vector2<float> pos, InvaderType type);
    void handleFormationKill();
    void changeState(GameStateID stateID);
};

#endif // APP_H

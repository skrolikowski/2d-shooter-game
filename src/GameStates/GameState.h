#ifndef STATE_H
#define STATE_H

#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class App; // forward declaration (pointer reference only)

enum class GameStateID : uint8_t
{
    Title,
    Playing,
    PlayerDying,
    GameOver
};

class GameState
{
  public:
    GameState() = default;
    GameState(App *app) : app_(app) {}
    virtual ~GameState() {}

    virtual void HandleInput() {}
    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Update(float /*dt*/) {}
    virtual void Draw(SDL_Renderer * /*renderer*/) const {}

  protected:
    App *app_ = nullptr;
};

#endif // STATE_H

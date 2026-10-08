#ifndef STATE_H
#define STATE_H

#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class App; // forward declaration (pointer reference only)

enum class GameStateID { Title, Playing, PlayerDying, GameOver };

class GameState
{
public:
    GameState(App* app) : app_(app) { }
    virtual ~GameState() { }

    virtual void HandleInput() {}
    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Update(float) {}
    virtual void Draw(SDL_Renderer*) const {}

protected:
    App* app_ = nullptr;
};

#endif // STATE_H

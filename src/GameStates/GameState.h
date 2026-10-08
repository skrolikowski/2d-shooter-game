#ifndef STATE_H
#define STATE_H

#include <stdio.h>
#include <format>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Texture.h"
#include "Player.h"

class App; // forward declaration (pointer reference only)

enum class GameStateID { Title, Playing, PlayerDying, GameOver };

class GameState
{
public:
    GameState(App* app) : app_(app) { }
    virtual ~GameState() { }

    virtual void HandleInput() {}
    virtual void Enter() { printf("Entering Game State"); }
    virtual void Exit() { printf("Exiting Game State"); }
    virtual void Update(float dt) { }
    virtual void Draw(SDL_Renderer* renderer) const { }

protected:
    App* app_ = nullptr;
};

#endif // STATE_H

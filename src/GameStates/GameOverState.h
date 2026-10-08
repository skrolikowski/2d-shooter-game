#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <format>

#include "App.h"
#include "GameState.h"

class GameOverState : public GameState
{
public:
    using GameState::GameState;
    ~GameOverState() { }

    void Enter() override;
    void Exit() override;
    void HandleInput() override;
    void Update(float dt) override;
    void Draw(SDL_Renderer* renderer) const override;

private:
    TTF_Text* titleText_;
    TTF_Text* subtitleText_;
};

#endif // GAMEOVERSTATE_H

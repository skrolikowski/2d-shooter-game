#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"

class GameOverState : public GameState
{
public:
    using GameState::GameState;
    ~GameOverState() {}

    void Enter() override;
    void HandleInput() override;
    void Draw(SDL_Renderer* renderer) const override;

private:
    TTF_TextPtr titleText_;
    TTF_TextPtr subtitleText_;
};

#endif // GAMEOVERSTATE_H

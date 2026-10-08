#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include <format>

#include "App.h"
#include "GameState.h"

class PlayingState : public GameState
{
public:
    using GameState::GameState;
    ~PlayingState() { }

    void Enter() override;
    void Exit() override;
    void HandleInput() override;
    void Update(float dt) override;
    void Draw(SDL_Renderer* renderer) const override;

private:
    void startNewGame();
    void resetPlayerPosition();

};

#endif // PLAYINGSTATE_H

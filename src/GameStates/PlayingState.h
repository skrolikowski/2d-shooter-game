#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState.h"

class PlayingState : public GameState
{
public:
    using GameState::GameState;
    ~PlayingState() {}

    void Enter() override;
    void HandleInput() override;
    
private:
    void startNewGame();
    void resetPlayerPosition();

};

#endif // PLAYINGSTATE_H

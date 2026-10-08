#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "GameState.h"

class TitleState : public GameState
{
public:
    using GameState::GameState;
    ~TitleState() {}

    void Enter() override;
    void Exit() override;
    void HandleInput() override;
    void Draw(SDL_Renderer* renderer) const override;

private:
    TTF_Text* titleText_;
    TTF_Text* subtitleText_;
};

#endif // TITLESTATE_H

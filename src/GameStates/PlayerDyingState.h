#ifndef PLAYERDYINGSTATE_H
#define PLAYERDYINGSTATE_H

#include "GameState.h"

class PlayerDyingState : public GameState
{
public:
    using GameState::GameState;
    ~PlayerDyingState() {}

    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void Draw(SDL_Renderer* renderer) const override;

private:
    float stateTimer_ = 0.f;
    float stateEndTime_ = 2.5f;

    TTF_Text* titleText_;
    TTF_Text* subtitleText_;

};

#endif // PLAYERDYINGSTATE_H

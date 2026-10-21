#ifndef PLAYERDYINGSTATE_H
#define PLAYERDYINGSTATE_H

#include "GameState.h"
#include "Util/SDLPtr.h"

class PlayerDyingState : public GameState
{
  public:
    using GameState::GameState;
    ~PlayerDyingState() {}

    void Enter() override;
    void Update(float dt) override;
    void Draw(SDL_Renderer *renderer) const override;

  private:
    float stateTimer_ = 0.F;
    float stateEndTime_ = 2.0F;

    TTF_TextPtr titleText_;
    TTF_TextPtr subtitleText_;
};

#endif // PLAYERDYINGSTATE_H

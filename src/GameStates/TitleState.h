#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "GameState.h"
#include "Util/SDLPtr.h"

class TitleState : public GameState
{
  public:
    using GameState::GameState;
    ~TitleState() {}

    void Enter() override;
    void HandleInput() override;
    void Draw(SDL_Renderer *renderer) const override;

  private:
    TTF_TextPtr titleText_;
    TTF_TextPtr subtitleText_;
};

#endif // TITLESTATE_H

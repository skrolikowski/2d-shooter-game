#ifndef WINSTATE_H
#define WINSTATE_H

#include "GameState.h"
#include "Util/SDLPtr.h"

class WinState : public GameState
{
  public:
    using GameState::GameState;
    ~WinState() {}

    void Enter() override;
    void HandleInput() override;
    void Draw(SDL_Renderer *renderer) const override;

  private:
    TTF_TextPtr titleText_;
    TTF_TextPtr subtitleText_;
};

#endif // WINSTATE_H

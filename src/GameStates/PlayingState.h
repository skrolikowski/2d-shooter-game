#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState.h"
#include "Util/SDLPtr.h"

class PlayingState : public GameState
{
  public:
    using GameState::GameState;
    ~PlayingState() {}

    void StartNewWave();
    void Enter() override;
    void Update(float dt) override;
    void HandleInput() override;
    void Draw(SDL_Renderer *renderer) const override;

  private:
    TTF_TextPtr levelText_;
    TTF_TextPtr waveText_;
    float waveTextTimer_ = 0.F;
    float waveTextInterval_ = 1.5F;

    void startNewGame();
    void resetPlayerPosition();
};

#endif // PLAYINGSTATE_H

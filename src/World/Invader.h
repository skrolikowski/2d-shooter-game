#ifndef INVADER_H
#define INVADER_H

#include "Entity.h"

class Invader : public Entity
{
  public:
    using Entity::Entity;

    void SetType(InvaderType type);
    [[nodiscard]] InvaderType GetType() const { return type_; }

    void SetState(InvaderState state) { state_ = state; }
    [[nodiscard]] InvaderState GetState() const { return state_; };

  private:
    InvaderState state_{};
    InvaderType type_{};
};

#endif // INVADER_H

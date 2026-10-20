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

    void SetHomePosition(Vector2<float> home) { home_ = home; }
    [[nodiscard]] const Vector2<float> &GetHomePosition() const { return home_; }

    void Update(float dt) override;

  private:
    InvaderState state_{};
    InvaderType type_{};

    float diveT_ = 0.F;
    float returnT_ = 0.F;
    Vector2<float> home_;
};

#endif // INVADER_H

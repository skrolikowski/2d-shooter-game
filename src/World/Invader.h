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
    void SetControlPoints(
        Vector2<float> p0, Vector2<float> p1, Vector2<float> p2, Vector2<float> p3)
    {
        p0_ = p0;
        p1_ = p1;
        p2_ = p2;
        p3_ = p3;
    }

    void Update(float dt) override;

  private:
    InvaderState state_{};
    InvaderType type_{};

    float diveT_ = 0.F;
    float returnT_ = 0.F;
    Vector2<float> p0_;
    Vector2<float> p1_;
    Vector2<float> p2_;
    Vector2<float> p3_;
    Vector2<float> home_;
};

#endif // INVADER_H

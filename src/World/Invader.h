#ifndef INVADER_H
#define INVADER_H

#include "Entity.h"

class Invader : public Entity
{
  public:
    using Entity::Entity;

    void SetType(InvaderType type);
    [[nodiscard]] InvaderType GetType() const { return type_; }

  private:
    InvaderType type_{};
};

#endif // INVADER_H

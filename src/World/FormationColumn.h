#ifndef FORMATION_COLUMN_H
#define FORMATION_COLUMN_H

#include <nlohmann/json_fwd.hpp>
#include <vector>
using json = nlohmann::json;

#include "Entity.h"

class FormationColumn
{
  public:
    FormationColumn(Vector2<float> pos, const json &data);
    /**
     *  Dev-Note:
     *  Declaring your own destructor, even an empty one, stops the compiler from generating move
     * operations.
     */
    // ~FormationColumn() {}

    [[nodiscard]] const Entity *GetLowestInvader() const;
    Entity *TryHit(const SDL_FRect &other);
    [[nodiscard]] bool CheckForStepFormationHit(float dx) const;
    void StepFormation(float dx, float dy, bool hitDetected = false);

    [[nodiscard]] int Size() const;
    void Clear();
    void Draw() const;

  private:
    std::vector<Entity> invaders_;

    int dir_ = 1;
};

#endif // FORMATION_COLUMN_H

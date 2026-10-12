#ifndef FORMATION_COLUMN_H
#define FORMATION_COLUMN_H

#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

#include "Defs.h"
#include "Entity.h"

class FormationColumn
{
public:
    FormationColumn(float x, float y, const json& data);
    ~FormationColumn() { }

    const Entity* GetLowestInvader() const;
    Entity* TryHit(const SDL_FRect& other);
    bool CheckForStepFormationHit(float dx) const;
    void StepFormation(float dx, float dy, bool hitDetected = false);

    int Size() const;
    void Clear();
    void Draw() const;

private:
    std::vector<Entity> invaders_;

    int dir_ = 1;
};


#endif // FORMATION_COLUMN_H

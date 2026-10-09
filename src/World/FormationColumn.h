#ifndef FORMATION_COLUMN_H
#define FORMATION_COLUMN_H

#include "Defs.h"
#include "Entity.h"
#include "Util/TextureManager.h"

#include <algorithm>
#include <memory>
#include <vector>

class FormationColumn
{
public:
    FormationColumn(int numRows, float x, float startY, TextureManager& textures);
    ~FormationColumn() { }

    const Entity* GetLowestInvader() const;
    bool TryHit(const SDL_FRect& other);
    bool CheckForStepFormationHit(float dx) const;
    void StepFormation(float dx, float dy, bool hitDetected = false);

    int Size() noexcept { return invaders_.size(); }
    void Clear();
    void Draw(SDL_Renderer* renderer) const;

private:
    std::vector<Entity> invaders_;

    int dir_ = 1;
};


#endif // FORMATION_COLUMN_H

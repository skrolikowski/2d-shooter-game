#ifndef ENTITY_H
#define ENTITY_H

#include <SDL3/SDL.h>

#include <memory>

#include "Defs.h"
#include "Util/Texture.h"
#include "Util/Vector2.h"

class World;   // forward declaration, NOT #include "World.h"

enum class EntityType { Player, Bullet, Invader };

class Entity
{
public:
    Entity() = default;
    Entity(const Vector2<float>& pos) : pos_(pos) { };
    virtual ~Entity() = default;

    void SetPosition(Vector2<float> pos) { pos_ = pos; }
    void SetTexture(std::shared_ptr<Texture> texture) { texture_ = texture; }
    void SetHealth(int health) noexcept { health_ = health; }
    void SetWorld(World* world) noexcept { world_ = world; }

    void SetType(EntityType type) { type_ = type; }
    EntityType GetType() const { return type_; }

    void SetDir(Vector2<float> dir) { dir_ = dir; }
    
    void SetX(float x) noexcept { pos_.x = x; }
    void SetY(float y) noexcept { pos_.y = y; }
    float GetX() const { return pos_.x; }
    float GetY() const { return pos_.y; }
    SDL_FRect GetRect() const { return SDL_FRect{ .x = pos_.x, .y = pos_.y, .w = width_, .h = height_ }; }
    virtual float GetWidth() const { return width_; }
    virtual float GetHeight() const { return height_; }

    void Damage(int amount);
    bool IsDead() const noexcept { return health_ <= 0; }

    virtual void HandleInput() {}  // Player Only!!
    virtual void Update(float dt);
    virtual void Draw(SDL_Renderer* renderer) const;

protected:
    EntityType type_;
    int health_ = 1;
    float speed_ = 50.f;
    Vector2<float> dir_;
    Vector2<float> pos_;
    Vector2<float> vel_;
    float width_ = ENTITY_SIZE;
    float height_ = ENTITY_SIZE;

    World* world_ = nullptr;
    std::shared_ptr<Texture> texture_;

};

#endif // ENTITY_H
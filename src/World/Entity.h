#ifndef ENTITY_H
#define ENTITY_H

#include <SDL3/SDL.h>

#include <memory>
#include <string>

#include "Defs.h"
#include "Util/Vector2.h"

enum class InvaderType
{
    A,
    B,
    C
};

class World; // forward declaration, NOT #include "World.h"

class Entity
{
  public:
    Entity() = default;
    Entity(const Vector2<float> &pos) : pos_(pos) {};
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2<float> &GetPosition() const { return pos_; };
    [[nodiscard]] Vector2<float> GetCenter() const;
    void SetPosition(Vector2<float> pos) { pos_ = pos; }
    void SetHealth(int health) noexcept { health_ = health; }
    void SetWorld(World *world) noexcept { world_ = world; }

    void SetDir(Vector2<float> dir) { dir_ = dir; }
    void SetType(InvaderType type) { type_ = type; }
    [[nodiscard]] InvaderType GetType() const { return type_; }
    void SetTexture(const std::string &texture) { textureName_ = texture; }

    void SetX(float x) noexcept { pos_.x = x; }
    void SetY(float y) noexcept { pos_.y = y; }
    [[nodiscard]] float GetX() const { return pos_.x; }
    [[nodiscard]] float GetY() const { return pos_.y; }
    [[nodiscard]] SDL_FRect GetRect() const
    {
        return SDL_FRect{ .x = pos_.x, .y = pos_.y, .w = width_, .h = height_ };
    }
    [[nodiscard]] virtual float GetWidth() const { return width_; }
    [[nodiscard]] virtual float GetHeight() const { return height_; }

    void Damage(int amount);
    bool IsDead() const noexcept { return health_ <= 0; }

    virtual void HandleInput() {} // Player Only!!
    virtual void Update(float dt);
    virtual void Draw() const;

  protected:
    int health_ = 1;
    float speed_ = 50.F;
    Vector2<float> dir_;
    Vector2<float> pos_;
    Vector2<float> vel_;
    float width_ = ENTITY_SIZE;
    float height_ = ENTITY_SIZE;

    InvaderType type_;
    std::string textureName_;

    World *world_ = nullptr;
};

#endif // ENTITY_H
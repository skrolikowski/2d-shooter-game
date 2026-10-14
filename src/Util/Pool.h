#ifndef POOL_H
#define POOL_H

/**
 * One thing to watch: (Regarding EventListener / EventBus)
 * ----
 * If you ever subscribe from pooled objects (bullets, invaders in your Pool),
 * they're reused rather than destroyed, so their Subscriptions never fire.
 * You'd need to clear the subscriptions when an object goes back to the pool.
 */

#include <SDL3/SDL.h>

#include <array>
#include <cstddef>

#include "Poolable.h"
#include "Texture.h"
#include "Utility.h"
#include "Vector2.h"

struct Particle
{
    Vector2<float> pos;
    Vector2<float> vel;
    Vector2<int> size;
    float life;
    float maxLife;
    SDL_FColor color;
    void SetHealth(int h)
    {
        life = static_cast<float>(h);
        maxLife = static_cast<float>(h);
    }
    [[nodiscard]] bool IsDead() const { return life <= 0; }
    void SetPosition(Vector2<float> p) { pos = p; }

    void Update(float dt)
    {
        life -= dt * 1.5F;
        pos += vel * dt;
    }

    void Draw() const
    {
        float a = life / maxLife;
        SDL_SetRenderDrawBlendMode(Texture::Renderer, SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColorFloat(Texture::Renderer, color.r, color.g, color.b, a);
        const SDL_FRect fillRect = {
            pos.x,
            pos.y,
            static_cast<float>(size.x),
            static_cast<float>(size.y),
        };
        SDL_RenderFillRect(Texture::Renderer, &fillRect);
    }
};

template <Poolable T, std::size_t N> class Pool
{
  public:
    Pool() { Clear(); }

    [[nodiscard]] std::size_t Size() const { return N; }

    void Clear()
    {
        for (auto &item : items_)
            item.SetHealth(0);
    }

    T *Spawn(Vector2<float> pos)
    {
        for (auto &item : items_)
        {
            if (item.IsDead())
            {
                item = T{};
                item.SetPosition(pos);
                return &item;
            }
        }

        return nullptr;
    }

    template <typename F> void ForEach(F &&f) // For Update calls..
    {
        for (auto &item : items_)
            if (!item.IsDead())
                f(item);
    }

    template <typename F> void ForEach(F &&f) const // For Draw calls..
    {
        for (const auto &item : items_)
            if (!item.IsDead())
                f(item);
    }

  private:
    std::array<T, N> items_{};
};

#endif // POOL_H

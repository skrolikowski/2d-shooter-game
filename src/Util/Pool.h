#ifndef POOL_H
#define POOL_H

/**
 * One thing to watch: (Regarding EventListener / EventBus)
 * ----
 * If you ever subscribe from pooled objects (bullets, invaders in your Pool),
 * they're reused rather than destroyed, so their Subscriptions never fire.
 * You'd need to clear the subscriptions when an object goes back to the pool.
 */

#include <array>
#include <cstddef>

#include "Vector2.h"

template <typename T, std::size_t N> class Pool
{
  public:
    Pool() { Clear(); }

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

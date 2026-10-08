#ifndef POOL_H
#define POOL_H

#include <array>
#include <cstddef>

#include "Vector2.h"

template <typename T, std::size_t N>
class Pool
{
public:
    Pool()
    {
        Clear();
    }

    void Clear()
    {
        for (auto& item : items_)
            item.SetHealth(0);
    }

    T* Spawn(Vector2<float> pos)
    {
        for (auto& item : items_)
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

    template <typename F>
    void ForEach(F&& f)         // For Update calls..
    {
        for (auto& item : items_)
            if (!item.IsDead())
                f(item);
    }

    template <typename F>
    void ForEach(F&& f) const   // For Draw calls..
    {
        for (const auto& item : items_)
            if (!item.IsDead())
                f(item);
    }

private:
    std::array<T, N> items_{};

};

#endif // POOL_H

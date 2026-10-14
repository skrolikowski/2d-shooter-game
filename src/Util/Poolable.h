#ifndef POOLABLE_H
#define POOLABLE_H

#include <concepts>

#include "Vector2.h"

template <typename T>
concept Poolable = std::default_initializable<T> && requires(T t, const T ct, Vector2<float> pos) {
    t.SetPosition(pos);
    t.SetHealth(0);
    { ct.IsDead() } -> std::convertible_to<bool>;
};

#endif // POOLABLE_H
#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <utility>

template <typename T>
class Vector2
{
public:
    Vector2() = default;
    Vector2(T x, T y);

    T Dot(const Vector2& other) const;
    T Magnitude() const;

    T x = 0.0f;
    T y = 0.0f;

    void Reset();

    // -- Operator overloads..
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(const Vector2& other) const;
    Vector2 operator*(T s) const;
    Vector2 operator-() const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const Vector2& other);
    Vector2& operator*=(T s);
};

#endif // VECTOR2_H

#include "Vector2.tpp"
#ifndef VECTOR2_H
#define VECTOR2_H

template <typename T> class Vector2
{
  public:
    Vector2() = default;
    Vector2(T x, T y);

    T Dot(const Vector2 &other) const;
    T Magnitude() const;
    Vector2 Normalize() const;

    T x = 0.0F;
    T y = 0.0F;

    void Reset();

    // -- Operator overloads..
    Vector2 operator+(const Vector2 &other) const;
    Vector2 operator-(const Vector2 &other) const;
    Vector2 operator*(const Vector2 &other) const;
    Vector2 operator*(T s) const;
    Vector2 operator-() const;
    Vector2 &operator+=(const Vector2 &other);
    Vector2 &operator-=(const Vector2 &other);
    Vector2 &operator*=(const Vector2 &other);
    Vector2 &operator*=(T s);
};

#endif // VECTOR2_H

#include "Vector2.tpp"
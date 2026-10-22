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
    Vector2 Lerp(const Vector2 &other, float t) const;

    static Vector2 GetCubicBezierPoint(
        const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, float t);

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
    bool operator==(const Vector2 &other) const;
};

#endif // VECTOR2_H

#include "Vector2.tpp"

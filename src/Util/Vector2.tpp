#pragma once

#include "Vector2.h"
#include <cmath>

template <typename T> Vector2<T>::Vector2(T x, T y) : x(x), y(y) {}

template <typename T> T Vector2<T>::Dot(const Vector2 &other) const
{
    return (x * other.x) + (y * other.y);
}

template <typename T> T Vector2<T>::Magnitude() const
{
    return std::sqrt((x * x) + (y * y));
}

template <typename T> Vector2<T> Vector2<T>::Lerp(const Vector2 &other, float t) const
{
    return { x + ((other.x - x) * t), y + ((other.y - y) * t) };
}

template <typename T>
Vector2<T> Vector2<T>::GetCubicBezierPoint(
    const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3, float t)
{
    // Vector2 p01 = p0.Lerp(p1, t);
    // Vector2 p12 = p1.Lerp(p2, t);
    // Vector2 p23 = p2.Lerp(p3, t);

    // Vector2 p012 = p01.Lerp(p12, t);
    // Vector2 p123 = p12.Lerp(p23, t);

    // return p012.Lerp(p123, t);

    float u = 1.F - t;
    return (p0 * (u * u * u)) + (p1 * (3.F * u * u * t)) + (p2 * (3.F * u * t * t)) +
           (p3 * (t * t * t));
}

template <typename T> Vector2<T> Vector2<T>::Normalize() const
{
    float len = Magnitude();
    if (len > 0.0F)
    {
        return { x / len, y / len };
    }
    return {};
}

template <typename T> void Vector2<T>::Reset()
{
    x = 0;
    y = 0;
}

template <typename T> Vector2<T> Vector2<T>::operator+(const Vector2 &other) const
{
    Vector2 v = *this;
    v += other;
    return v;
}

template <typename T> Vector2<T> Vector2<T>::operator-(const Vector2 &other) const
{
    Vector2 v = *this;
    v -= other;
    return v;
}

template <typename T> Vector2<T> Vector2<T>::operator*(const Vector2 &other) const
{
    Vector2 v = *this;
    v *= other;
    return v;
}

template <typename T> Vector2<T> Vector2<T>::operator*(T s) const
{
    Vector2 v = *this;
    v *= s;
    return v;
}

template <typename T> Vector2<T> Vector2<T>::operator-() const
{
    return { -x, -y };
}

template <typename T> Vector2<T> &Vector2<T>::operator-=(const Vector2 &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

template <typename T> Vector2<T> &Vector2<T>::operator+=(const Vector2 &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

template <typename T> Vector2<T> &Vector2<T>::operator*=(const Vector2 &other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

template <typename T> Vector2<T> &Vector2<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    return *this;
}
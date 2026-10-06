#pragma once

#include "Vector2.h"

template <typename T>
Vector2<T>::Vector2(T x, T y)
    : x(x), y(y) { }

template <typename T>
T Vector2<T>::Dot(const Vector2& other) const
{
    return x * other.x + y * other.y;
}

template <typename T>
T Vector2<T>::Magnitude() const
{
    return std::sqrt(x * x + y * y);
}

template <typename T>
void Vector2<T>::Reset()
{
    x = 0;
    y = 0;
}

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2& other) const
{
    Vector2 v = *this;
    v += other;
    return v;
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2& other) const
{
    Vector2 v = *this;
    v -= other;
    return v;
}

template <typename T>
Vector2<T> Vector2<T>::operator*(const Vector2& other) const
{
    Vector2 v = *this;
    v *= other;
    return v;
}

template <typename T>
Vector2<T> Vector2<T>::operator*(T s) const
{
    Vector2 v = *this;
    v *= s;
    return v;
}

template <typename T>
Vector2<T> Vector2<T>::operator-() const
{
    return {-x, -y};
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(const Vector2& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    return *this;
}
namespace Easing
{
inline float Linear(float t)
{
    return t;
}

inline float InQuad(float t)
{
    return t * t;
}

inline float OutQuad(float t)
{
    return 1.F - (1.F - t) * (1.F - t);
}

inline float InCubic(float t)
{
    return t * t * t;
}
} // namespace Easing
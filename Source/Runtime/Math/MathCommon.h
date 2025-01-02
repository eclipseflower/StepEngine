#pragma once

#define PI 3.14159265358979323846f

namespace StepEngine
{
    inline static constexpr float DegToRad(float degree)
    {
        return degree * (PI / 180.0f);
    }

    inline static constexpr float RadToDeg(float radian)
    {
        return radian * (180.0f / PI);
    }

    template<typename T>
    inline static constexpr T Clamp(T value, T min, T max)
    {
        return value < min ? min : value > max ? max : value;
    }

    template<typename T>
    inline static constexpr T Clamp01(T value)
    {
        return Clamp(value, static_cast<T>(0), static_cast<T>(1));
    }

    template<typename T, typename U>
    inline static constexpr T Lerp(T a, T b, U t)
    {
        t = Clamp01(t);
        return (1 - t) * a + t * b;
    }
}

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
}

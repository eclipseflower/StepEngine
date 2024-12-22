#pragma once

#include <cmath>

namespace StepEngine
{
    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        float Length() const { return sqrt(x * x + y * y + z * z); }
        Vector3& RotateAroundAxis(const float angle, const Vector3& axis);
    };
}

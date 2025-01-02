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

        Vector3 operator+(const Vector3& v) const
        {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }
    };

    inline Vector3 operator*(const Vector3& v, float s)
    {
        return Vector3(v.x * s, v.y * s, v.z * s);
    }
    inline Vector3 operator*(float s, const Vector3& v)
    {
        return v * s;
    }
}

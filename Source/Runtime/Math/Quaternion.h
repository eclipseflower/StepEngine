#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

namespace StepEngine
{
    struct Quaternion
    {
        float x, y, z, w;

        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Quaternion(Matrix4x4 m);

        Vector3 Rotate(const Vector3& v) const;

        Matrix4x4 ToMatrix() const;
    };
}

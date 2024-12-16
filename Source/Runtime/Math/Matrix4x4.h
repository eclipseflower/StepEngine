#pragma once

#include "Vector3.h"

namespace StepEngine
{
    struct Matrix4x4
    {
        float m[16];

        float& Get(int row, int column) { return m[column * 4 + row]; }
        const float& Get(int row, int column) const { return m[column * 4 + row]; }

        Vector3 GetTranslation() const;
        Vector3 GetScale() const;

        Matrix4x4& SetTranslation(const Vector3& translation);
        Matrix4x4& SetScale(const Vector3& scale);
    };
}

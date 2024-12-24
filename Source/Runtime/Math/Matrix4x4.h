#pragma once

#include "Vector3.h"

namespace StepEngine
{
    struct Matrix4x4
    {
        float m[16];

        float& Get(int row, int column) { return m[column * 4 + row]; }
        const float& Get(int row, int column) const { return m[column * 4 + row]; }
        
        Vector3 GetAxisX() const;
        Vector3 GetAxisY() const;
        Vector3 GetAxisZ() const;

        Vector3 GetTranslation() const;
        Vector3 GetScale() const;
        Vector3 GetRotation() const;

        Matrix4x4& SetTranslation(const Vector3& translation);
        Matrix4x4& SetScale(const Vector3& scale);
        Matrix4x4& SetRotation(const Vector3& rotation);
        Matrix4x4& SetTRS(const Vector3& translation, const Vector3& rotation, const Vector3& scale);
        Matrix4x4& RotateAroundAxis(const float angle, const Vector3& axis);

        Vector3 MultiplyVector(const Vector3& v) const;
        Vector3 MultiplyPoint(const Vector3& v) const;
    };
}

#include "Matrix4x4.h"

namespace StepEngine
{
    Vector3 Matrix4x4::GetTranslation() const
    {
        return Vector3(Get(0, 3), Get(1, 3), Get(2, 3));
    }
    Vector3 Matrix4x4::GetScale() const
    {
        return { m[0][0], m[1][1], m[2][2] };
    }
    Matrix4x4& Matrix4x4::SetTranslation(const Vector3& translation)
    {
        Get(0, 3) = translation.x;
        Get(1, 3) = translation.y;
        Get(2, 3) = translation.z;
        Get(3, 3) = 1.0f;
        return *this;
    }
    Matrix4x4& Matrix4x4::SetScale(const Vector3& scale)
    {
        m[0][0] = scale.x;
        m[1][1] = scale.y;
        m[2][2] = scale.z;
        return *this;
    }
}

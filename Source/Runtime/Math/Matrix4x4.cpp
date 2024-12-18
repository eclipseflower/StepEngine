#include "Matrix4x4.h"
#include "Runtime/Math/Vector3.h"

namespace StepEngine
{
    Vector3 Matrix4x4::GetAxisX() const
    {
        return Vector3(Get(0, 0), Get(1, 0), Get(2, 0));
    }
    Vector3 Matrix4x4::GetAxisY() const
    {
        return Vector3(Get(0, 1), Get(1, 1), Get(2, 1));
    }
    Vector3 Matrix4x4::GetAxisZ() const
    {
        return Vector3(Get(0, 2), Get(1, 2), Get(2, 2));
    }
    Vector3 Matrix4x4::GetTranslation() const
    {
        return Vector3(Get(0, 3), Get(1, 3), Get(2, 3));
    }
    Vector3 Matrix4x4::GetScale() const
    {
        float x = GetAxisX().Length();
        float y = GetAxisY().Length();
        float z = GetAxisZ().Length();
        return Vector3(x, y, z);
    }
    Matrix4x4& Matrix4x4::SetTranslation(const Vector3& translation)
    {
        Get(0, 0) = 1.0f;
        Get(1, 0) = 0.0f;
        Get(2, 0) = 0.0f;
        Get(3, 0) = 0.0f;

        Get(0, 1) = 0.0f;
        Get(1, 1) = 1.0f;
        Get(2, 1) = 0.0f;
        Get(3, 1) = 0.0f;

        Get(0, 2) = 0.0f;
        Get(1, 2) = 0.0f;
        Get(2, 2) = 1.0f;
        Get(3, 2) = 0.0f;

        Get(0, 3) = translation.x;
        Get(1, 3) = translation.y;
        Get(2, 3) = translation.z;
        Get(3, 3) = 1.0f;

        return *this;
    }
    Matrix4x4& Matrix4x4::SetScale(const Vector3& scale)
    {
        Get(0, 0) = scale.x;
        Get(1, 0) = 0.0f;
        Get(2, 0) = 0.0f;
        Get(3, 0) = 0.0f;

        Get(0, 1) = 1.0f;
        Get(1, 1) = scale.y;
        Get(2, 1) = 0.0f;
        Get(3, 1) = 0.0f;

        Get(0, 2) = 1.0f;
        Get(1, 2) = 0.0f;
        Get(2, 2) = scale.z;
        Get(3, 2) = 0.0f;

        Get(0, 3) = 0;
        Get(1, 3) = 0;
        Get(2, 3) = 0;
        Get(3, 3) = 1.0f;

        return *this;
    }
    Matrix4x4& Matrix4x4::SetRotation(const Vector3& rotation)
    {
        float cx = cos(rotation.x);
        float sx = sin(rotation.x);
        float cy = cos(rotation.y);
        float sy = sin(rotation.y);
        float cz = cos(rotation.z);
        float sz = sin(rotation.z);
    
        Get(0, 0) = cy * cz;
        Get(1, 0) = cy * sz;
        Get(2, 0) = -sy;
        Get(3, 0) = 0.0f;
        Get(0, 1) = sx * sy * cz - cx * sz;
        Get(1, 1) = sx * sy * sz + cx * cz;
        Get(2, 1) = sx * cy;
        Get(3, 1) = 0.0f;
        Get(0, 2) = cx * sy * cz + sx * sz;
        Get(1, 2) = cx * sy * sz - sx * cz;
        Get(2, 2) = cx * cy;
        Get(3, 2) = 0.0f;
        Get(0, 3) = 0.0f;
        Get(1, 3) = 0.0f;
        Get(2, 3) = 0.0f;
        Get(3, 3) = 1.0f;
        return *this;
    }
}

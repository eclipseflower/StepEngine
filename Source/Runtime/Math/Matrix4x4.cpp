#include "Runtime/Math/MathCommon.h"
#include "Runtime/Math/Vector3.h"
#include "Matrix4x4.h"

namespace StepEngine
{
    const Matrix4x4 Matrix4x4::Identity = Matrix4x4().SetIdentity();

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
    Vector3 Matrix4x4::GetRotation() const
    {
        float x = RadToDeg(atan2(Get(2, 1), Get(2, 2)));
        float y = RadToDeg(asin(-Get(2, 0)));
        float z = RadToDeg(atan2(Get(1, 0), Get(0, 0)));
        return Vector3(x, y, z);
    }
    Matrix4x4 Matrix4x4::GetTranspose() const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.Get(i, j) = Get(j, i);
            }
        }
        return result;
    }
    Matrix4x4& Matrix4x4::SetIdentity()
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

        Get(0, 3) = 0.0f;
        Get(1, 3) = 0.0f;
        Get(2, 3) = 0.0f;
        Get(3, 3) = 1.0f;

        return *this;
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

        Get(0, 1) = 0.0f;
        Get(1, 1) = scale.y;
        Get(2, 1) = 0.0f;
        Get(3, 1) = 0.0f;

        Get(0, 2) = 0.0f;
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
        float x = DegToRad(rotation.x);
        float y = DegToRad(rotation.y);
        float z = DegToRad(rotation.z);

        float cx = cos(x);
        float sx = sin(x);
        float cy = cos(y);
        float sy = sin(y);
        float cz = cos(z);
        float sz = sin(z);
    
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
    Matrix4x4& Matrix4x4::SetTRS(const Vector3& translation, const Vector3& rotation, const Vector3& scale)
    {
        float cx = cos(rotation.x);
        float sx = sin(rotation.x);
        float cy = cos(rotation.y);
        float sy = sin(rotation.y);
        float cz = cos(rotation.z);
        float sz = sin(rotation.z);
    
        Get(0, 0) = cy * cz * scale.x;
        Get(1, 0) = cy * sz * scale.x;
        Get(2, 0) = -sy * scale.x;
        Get(3, 0) = 0.0f;

        Get(0, 1) = (sx * sy * cz - cx * sz) * scale.y;
        Get(1, 1) = (sx * sy * sz + cx * cz) * scale.y;
        Get(2, 1) = sx * cy * scale.y;
        Get(3, 1) = 0.0f;

        Get(0, 2) = (cx * sy * cz + sx * sz) * scale.z;
        Get(1, 2) = (cx * sy * sz - sx * cz) * scale.z;
        Get(2, 2) = cx * cy * scale.z;
        Get(3, 2) = 0.0f;

        Get(0, 3) = translation.x;
        Get(1, 3) = translation.y;
        Get(2, 3) = translation.z;
        Get(3, 3) = 1.0f;

        return *this;
    }
    Matrix4x4& Matrix4x4::RotateAroundAxis(const float angle, const Vector3& axis)
    {
        float rad = DegToRad(angle);
        float s = sin(rad);
        float c = cos(rad);
        float omc = 1.0f - c;
        
        float xx = axis.x * axis.x;
        float yy = axis.y * axis.y;
        float zz = axis.z * axis.z;

        float xy = axis.x * axis.y;
        float yz = axis.y * axis.z;
        float xz = axis.x * axis.z;

        float xs = axis.x * s;
        float ys = axis.y * s;
        float zs = axis.z * s;

        Get(0, 0) = xx * omc + c;
        Get(1, 0) = xy * omc + zs;
        Get(2, 0) = xz * omc - ys;
        Get(3, 0) = 0.0f;

        Get(0, 1) = xy * omc - zs;
        Get(1, 1) = yy * omc + c;
        Get(2, 1) = yz * omc + xs;
        Get(3, 1) = 0.0f;

        Get(0, 2) = xz * omc + ys;
        Get(1, 2) = yz * omc - xs;
        Get(2, 2) = zz * omc + c;
        Get(3, 2) = 0.0f;

        Get(0, 3) = 0.0f;
        Get(1, 3) = 0.0f;
        Get(2, 3) = 0.0f;
        Get(3, 3) = 1.0f;

        return *this;
    }
    Vector3 Matrix4x4::MultiplyVector(const Vector3& v) const
    {
        float x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z;
        float y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z;
        float z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z;
        return Vector3(x, y, z);
    }
    Vector3 Matrix4x4::MultiplyPoint(const Vector3& v) const
    {
        float x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z + Get(0, 3);
        float y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z + Get(1, 3);
        float z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z + Get(2, 3);
        return Vector3(x, y, z);
    }
}

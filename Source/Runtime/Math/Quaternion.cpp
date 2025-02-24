#include "Quaternion.h"

namespace StepEngine
{
    Quaternion::Quaternion(Matrix4x4 m)
    {
        float trace = m.Get(0, 0) + m.Get(1, 1) + m.Get(2, 2);
        if (trace > 0.0f)
        {
            w = sqrt(trace + 1.0f) / 2.0f;
        }
    }

    Vector3 Quaternion::Rotate(const Vector3& v) const
    {
        Vector3 q(x, y, z);
        Vector3 t = Cross(q, v) * 2.0f;
        return v + t * w + Cross(q, t);
    }

    Matrix4x4 Quaternion::ToMatrix() const
    {
        const float x2 = x * 2.0f;
        const float y2 = y * 2.0f;
        const float z2 = z * 2.0f;

        const float xx = x * x2;
        const float xy = x * y2;
        const float xz = x * z2;

        const float yx = y * x2;
        const float yy = y * y2;
        const float yz = y * z2;

        const float zx = z * x2;
        const float zy = z * y2;
        const float zz = z * z2;

        const float wx = w * x2;
        const float wy = w * y2;
        const float wz = w * z2;

        Matrix4x4 m;
        m.Get(0, 0) = 1.0f - (yy + zz);
        m.Get(1, 0) = xy + wz;
        m.Get(2, 0) = xz - wy;
        m.Get(3, 0) = 0.0f;

        m.Get(0, 1) = xy - wz;
        m.Get(1, 1) = 1.0f - (xx + zz);
        m.Get(2, 1) = yz + wx;
        m.Get(3, 1) = 0.0f;

        m.Get(0, 2) = xz + wy;
        m.Get(1, 2) = yz - wx;
        m.Get(2, 2) = 1.0f - (xx + yy);
        m.Get(3, 2) = 0.0f;

        m.Get(0, 3) = 0.0f;
        m.Get(1, 3) = 0.0f;
        m.Get(2, 3) = 0.0f;
        m.Get(3, 3) = 1.0f;

        return m;
    }
}

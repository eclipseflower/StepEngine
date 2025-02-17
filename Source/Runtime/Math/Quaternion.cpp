#include "Quaternion.h"

namespace StepEngine
{
    Vector3 Quaternion::Rotate(const Vector3& v) const
    {
        Vector3 q(x, y, z);
        Vector3 t = Cross(q, v) * 2.0f;
        return v + t * w + Cross(q, t);
    }

    Matrix4x4 Quaternion::ToMatrix() const
    {

    }
}

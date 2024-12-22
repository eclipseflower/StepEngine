#include "Vector3.h"

namespace StepEngine
{
    Vector3& Vector3::RotateAroundAxis(const float angle, const Vector3& axis)
    {
        float s = sin(angle);
        float c = cos(angle);
        
        float xx = axis.x * axis.x;
        float yy = axis.y * axis.y;
        float zz = axis.z * axis.z;

        float xy = axis.x * axis.y;
        float yz = axis.y * axis.z;
        float xz = axis.x * axis.z;
    }
}

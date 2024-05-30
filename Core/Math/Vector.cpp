#include "Vector.h"

namespace Step
{
    template<> const Vector3f Vector3f::zero = Vector3f(0, 0, 0);
    template<> const Vector3f Vector3f::one = Vector3f(1, 1, 1);
    template<> const Vector3f Vector3f::up = Vector3f(0, 1, 0);
    template<> const Vector3f Vector3f::down = Vector3f(0, -1, 0);
    template<> const Vector3f Vector3f::forward = Vector3f(0, 0, 1);
    template<> const Vector3f Vector3f::backward = Vector3f(0, 0, -1);
    template<> const Vector3f Vector3f::right = Vector3f(1, 0, 0);
    template<> const Vector3f Vector3f::left = Vector3f(-1, 0, 0);
    template<> const Vector3f Vector3f::xAxis = Vector3f(1, 0, 0);
    template<> const Vector3f Vector3f::yAxis = Vector3f(0, 1, 0);
    template<> const Vector3f Vector3f::zAxis = Vector3f(0, 0, 1);

    template<> const Vector3d Vector3d::zero = Vector3d(0, 0, 0);
    template<> const Vector3d Vector3d::one = Vector3d(1, 1, 1);
    template<> const Vector3d Vector3d::up = Vector3d(0, 1, 0);
    template<> const Vector3d Vector3d::down = Vector3d(0, -1, 0);
    template<> const Vector3d Vector3d::forward = Vector3d(0, 0, 1);
    template<> const Vector3d Vector3d::backward = Vector3d(0, 0, -1);
    template<> const Vector3d Vector3d::right = Vector3d(1, 0, 0);
    template<> const Vector3d Vector3d::left = Vector3d(-1, 0, 0);
    template<> const Vector3d Vector3d::xAxis = Vector3d(1, 0, 0);
    template<> const Vector3d Vector3d::yAxis = Vector3d(0, 1, 0);
    template<> const Vector3d Vector3d::zAxis = Vector3d(0, 0, 1);
}
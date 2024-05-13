#ifndef _VECTOR_H_
#define _VECTOR_H_

namespace Step
{
    template<typename T>
    struct Vector
    {
        static_assert(std::is_floating_point_v<T>, "Vector type must be float");

        union
        {
            struct
            {
                T x, y, z;
            };
            T xyz[3];
        };

        static const Vector<T> zero;
        static const Vector<T> one;
        static const Vector<T> up;
        static const Vector<T> down;
        static const Vector<T> forward;
        static const Vector<T> backward;
        static const Vector<T> right;
        static const Vector<T> left;
        static const Vector<T> xAxis;
        static const Vector<T> yAxis;
        static const Vector<T> zAxis;

        inline Vector() {}
        inline Vector(T inX, T inY, T inZ) : x(inX), y(inY), z(inZ) {}

        inline Vector<T> operator+ (const Vector<T>& v) const;
        inline Vector<T> operator- (const Vector<T>& v) const;

        template<typename ARGTYPE>
        inline Vector<T> operator* (ARGTYPE scale) const requires std::is_arithmetic_v<T>
        {
            return Vector<T>(x * static_cast<T>(scale), y * static_cast<T>(scale), z * static_cast<T>(scale));
        }

        inline static Vector<T> Cross(const Vector<T>& lhs, const Vector<T>& rhs);
        inline static T Dot(const Vector<T>& lhs, const Vector<T>& rhs);
    };

    template<typename T>
    inline Vector<T> Vector<T>::operator+(const Vector<T>& v) const
    {
        return Vector<T>(x + v.x, y + v.y, z + v.z);
    }

    template<typename T>
    inline Vector<T> Vector<T>::operator-(const Vector<T>& v) const
    {
        return Vector<T>(x - v.x, y - v.y, z - v.z);
    }

    template<typename T>
    inline Vector<T> Cross(const Vector<T>& lhs, const Vector<T>& rhs)
    {
        return Vector<T>
        {
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        };
    }

    template<typename T>
    inline T Dot(const Vector<T>& lhs, const Vector<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }
}

#endif // _VECTOR_H_
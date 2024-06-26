#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "MathForward.h"
#include <type_traits>

#define VECTOR_EPSILON (1.e-4f)

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

        inline T& operator[](int index);
        inline const T& operator[](int index) const;

        inline Vector<T> operator+ (const Vector<T>& v) const;
        inline Vector<T> operator- (const Vector<T>& v) const;
        inline Vector<T> operator* (const Vector<T>& v) const;
        inline Vector<T> operator/ (const Vector<T>& v) const;
        inline Vector<T> operator- () const;

        template<typename ARGTYPE>
        inline Vector<T> operator* (ARGTYPE scale) const requires std::is_arithmetic_v<ARGTYPE>
        {
            return Vector<T>(x * scale, y * scale, z * scale);
        }

        template<typename ARGTYPE>
        inline Vector<T> operator/ (ARGTYPE scale) const requires std::is_arithmetic_v<ARGTYPE>
        {
            const T rScale = 1 / static_cast<T>(scale);
            return Vector<T>(x * rScale, y * rScale, z * rScale);
        }

        inline Vector<T>& operator+= (const Vector<T>& v);
        inline Vector<T>& operator-= (const Vector<T>& v);

        inline bool operator== (const Vector<T>& v) const;
        inline bool operator!= (const Vector<T>& v) const;

        template<typename ARGTYPE>
        inline Vector<T>& operator*= (ARGTYPE scale) requires std::is_arithmetic_v<ARGTYPE>
        {
            x *= scale;
            y *= scale;
            z *= scale;
            return *this;
        }

        template<typename ARGTYPE>
        inline Vector<T>& operator/= (ARGTYPE scale) requires std::is_arithmetic_v<ARGTYPE>
        {
            const T rScale = 1 / static_cast<T>(scale);
            x *= rScale;
            y *= rScale;
            z *= rScale;
            return *this;
        }

        inline bool Equals(const Vector<T>& v, const T tolerance = VECTOR_EPSILON) const;
        inline void Set(T inX, T inY, T inZ);
        inline T Length() const;
        inline T SquaredLength() const;
        inline bool IsNormalized(const T tolerance = VECTOR_EPSILON) const;
        inline Vector<T> Normalize(const T tolerance = VECTOR_EPSILON, const Vector<T>& defaultV = zero) const;

        inline static Vector<T> Cross(const Vector<T>& lhs, const Vector<T>& rhs);
        inline static T Dot(const Vector<T>& lhs, const Vector<T>& rhs);
    };

    template<typename T>
    inline T& Vector<T>::operator[](int index)
    {
        return xyz[index];
    }

    template<typename T>
    inline const T& Vector<T>::operator[](int index) const
    {
        return xyz[index];
    }

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
    inline Vector<T> Vector<T>::operator*(const Vector<T>& v) const
    {
        return Vector<T>(x * v.x, y * v.y, z * v.z);
    }

    template<typename T>
    inline Vector<T> Vector<T>::operator/(const Vector<T>& v) const
    {
        return Vector<T>(x / v.x, y / v.y, z / v.z);
    }

    template<typename T>
    inline Vector<T> Vector<T>::operator-() const
    {
        return Vector<T>(-x, -y, -z);
    }

    template<typename T>
    inline Vector<T>& Vector<T>::operator+=(const Vector<T>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename T>
    inline Vector<T>& Vector<T>::operator-=(const Vector<T>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template<typename T>
    inline bool Vector<T>::operator==(const Vector<T>& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    template<typename T>
    inline bool Vector<T>::operator!=(const Vector<T>& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    template<typename T>
    inline bool Vector<T>::Equals(const Vector<T>& v, const T tolerance) const
    {
        return std::abs(x - v.x) <= tolerance && std::abs(y - v.y) <= tolerance && std::abs(z - v.z) <= tolerance;
    }

    template<typename T>
    inline void Vector<T>::Set(T inX, T inY, T inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }

    template<typename T>
    inline T Vector<T>::Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    template<typename T>
    inline T Vector<T>::SquaredLength() const
    {
        return x * x + y * y + z * z;
    }

    template<typename T>
    inline bool Vector<T>::IsNormalized(const T tolerance) const
    {
        return std::abs(SquaredLength() - 1) <= tolerance;
    }

    template<typename T>
    inline Vector<T> Vector<T>::Normalize(const T tolerance, const Vector<T>& defaultV) const
    {
        const T squareLen = SquaredLength();
        if (squareLen <= tolerance)
        {
            return defaultV;
        }
        const T invLen = 1 / sqrt(squareLen);
        return Vector<T>(x * invLen, y * invLen, z * invLen);
    }

    template<typename T>
    inline Vector<T> Vector<T>::Cross(const Vector<T>& lhs, const Vector<T>& rhs)
    {
        return Vector<T>
        {
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        };
    }

    template<typename T>
    inline T Vector<T>::Dot(const Vector<T>& lhs, const Vector<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }
}

#endif // _VECTOR_H_
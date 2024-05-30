#ifndef _MATRIX_H_
#define _MATRIX_H_

namespace Step
{
    template<typename T>
    struct alignas(16) Matrix
    {
        static_assert(std::is_floating_point_v<T>, "Matrix type must be float");

        alignas(16) T m[4][4];
    };
}

#endif // _MATRIX_H_
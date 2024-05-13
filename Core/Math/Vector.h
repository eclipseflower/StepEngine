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

    }
}

#endif // _VECTOR_H_
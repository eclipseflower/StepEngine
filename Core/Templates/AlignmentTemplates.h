#include <cstdint>

namespace Step
{
    template<typename T>
    __forceinline constexpr T Align(T val, uint64_t alignment)
    {
        return reinterpret_cast<T>((reinterpret_cast<uint64_t>(val) + alignment) & ~(alignment - 1));
    }
}
#include <cstdint>

namespace Step
{
    static constexpr uint8_t DEFAULT_ALIGNMENT = 0;
    static constexpr uint8_t MIN_ALIGNMENT = 8;

    class MemoryMalloc
    {
    public:
        virtual void* Malloc(size_t count, uint32_t alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void* Realloc(void* pOriginal, size_t count, uint32_t alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void Free(void* pOriginal) = 0;
    };
}
#include <cstdint>

namespace Step
{
    static constexpr uint32_t DEFAULT_ALIGNMENT = 0;
    static constexpr uint32_t ALIGNMENT_8 = 8;
    static constexpr uint32_t ALIGNMENT_16 = 16;

    class MemoryMalloc
    {
    public:
        virtual void* Malloc(size_t count, uint32_t alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void* Realloc(void* pOriginal, size_t count, uint32_t alignment = DEFAULT_ALIGNMENT) = 0;
        virtual void Free(void* pOriginal) = 0;
    };
}
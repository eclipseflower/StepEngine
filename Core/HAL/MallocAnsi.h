#include "MemoryBase.h"

namespace Step
{
    void* AnsiMalloc(size_t count, uint32_t alignment);
    void* AnsiRealloc(void* pOriginal, size_t count, uint32_t alignment);
    void AnsiFree(void* ptr);

    class MallocAnsi final : public MemoryMalloc
    {
    public:
        virtual void* Malloc(size_t count, uint32_t alignment) override;
        virtual void* Realloc(void* pOriginal, size_t count, uint32_t alignment) override;
        virtual void Free(void* pOriginal) override;
    };
}
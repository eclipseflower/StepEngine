#include "MallocAnsi.h"
#include <malloc.h>

namespace Step
{
    void* AnsiMalloc(size_t count, uint32_t alignment)
    {
        void* ptr = malloc(count + alignment + sizeof(void *) + sizeof(size_t));
        void* pResult = nullptr;
        if(ptr)
        {
            pResult = Align(reinterpret_cast<uint8_t *>(ptr) + sizeof(void *) + sizeof(size_t), alignment);
            *(reinterpret_cast<void **>(reinterpret_cast<uint8_t*>(pResult) - sizeof(void *))) = ptr;
            *(reinterpret_cast<size_t*>(reinterpret_cast<uint8_t*>(pResult) - sizeof(void *) - sizeof(size_t))) = count;
        }

        return pResult;
    }

    void* AnsiRealloc(void* pOriginal, size_t count, uint32_t alignment)
    {
        void* pResult = nullptr;
        if(pOriginal && count > 0)
        {
            pResult = AnsiMalloc(count, alignment);
            size_t oldSize = AnsiGetAllocationSize(pOriginal);
            memcpy(pResult, pOriginal, min(count, oldSize));
            AnsiFree(pOriginal);
        }
        else if(pOriginal == nullptr)
        {
            pResult = AnsiMalloc(count, alignment);
        }
        else
        {
            AnsiFree(pOriginal);
        }

        return pResult;
    }

    void AnsiFree(void* ptr)
    {
        if(ptr)
        {
            free(*(reinterpret_cast<void **>(reinterpret_cast<uint8_t*>(ptr) - sizeof(void *))));
        }
    }

    void* MallocAnsi::Malloc(size_t count, uint32_t alignment)
    {
        alignment = max(count >= 16 ? SIXTEEN_ALIGNMENT : EIGHT_ALIGNMENT, alignment);
        return AnsiMalloc(count, alignment);
    }

    void* MallocAnsi::Realloc(void* pOriginal, size_t count, uint32_t alignment)
    {
        alignment = max(count >= 16 ? SIXTEEN_ALIGNMENT : EIGHT_ALIGNMENT, alignment);
        return AnsiRealloc(pOriginal, count, alignment);
    }

    void MallocAnsi::Free(void* pOriginal)
    {
        AnsiFree(pOriginal);
    }
}
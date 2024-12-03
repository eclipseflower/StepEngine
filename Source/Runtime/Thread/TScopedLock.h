#pragma once
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine 
{
    template<typename T>
    class TScopedLock
    {
        NON_COPYABLE(TScopedLock);
    public:
        explicit TScopedLock(T& lock) : m_lock(&lock)
        {
            m_lock->Lock();
        }

        ~TScopedLock()
        {
            m_lock->Unlock();
        }

    private:
        T* m_lock;
    };
}

#pragma once

#include <atomic>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class ReentrantLock
    {
        NON_COPYABLE(ReentrantLock);
    public:
        ReentrantLock() = default;
        ~ReentrantLock() = default;
        bool TryLock();
        void Lock(unsigned int spinCount = 100);
        void Unlock();
    private:
        std::atomic<unsigned int> m_threadId = 0;
        std::atomic<unsigned int> m_refCount = 0;
    };
}

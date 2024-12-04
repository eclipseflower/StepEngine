#pragma once

#include <atomic>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class SpinLock
    {
        NON_COPYABLE(SpinLock);
    public:
        SpinLock() = default;
        ~SpinLock() = default;
        bool TryLock();
        void Lock(unsigned int spinCount = 100);
        void Unlock();
    private:
        std::atomic<bool> m_lock = false;
    };
}

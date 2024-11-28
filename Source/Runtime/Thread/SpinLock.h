#pragma once

#include <atomic>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class SpinCounter
    {
    public:
        SpinCounter(unsigned int maxSpinCount) : m_maxSpinCount(maxSpinCount), m_curSpinCount(0) {}
        void Spin();
    private:
        unsigned int m_maxSpinCount;
        unsigned int m_curSpinCount;
    };

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

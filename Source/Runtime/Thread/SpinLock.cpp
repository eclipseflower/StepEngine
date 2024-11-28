#include "SpinLock.h"
#include <atomic>

namespace StepEngine
{
    bool SpinLock::TryLock()
    {
        return !m_lock.exchange(true, std::memory_order_acquire);
    }
    void SpinLock::Lock(unsigned int spinCount)
    {
        if(TryLock())
        {
            return;
        }

        SpinCounter spinCounter(spinCount);
        while (m_lock.exchange(true, std::memory_order_relaxed))
        {
            spinCounter.Spin();
        }
    }
    void SpinLock::Unlock()
    {
        m_lock.store(false, std::memory_order_release);
    }
}

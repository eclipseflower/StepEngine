#include <atomic>
#include "SpinCounter.h"
#include "SpinLock.h"

namespace StepEngine
{
    bool SpinLock::TryLock()
    {
        return !m_lock.exchange(true, std::memory_order_acquire);
    }
    void SpinLock::Lock(unsigned int spinCount)
    {
        SpinCounter spinCounter(spinCount);
        while(!TryLock())
        {
            while (m_lock.load(std::memory_order_relaxed))
            {
                spinCounter.Spin();
            }
        }
    }
    void SpinLock::Unlock()
    {
        m_lock.store(false, std::memory_order_release);
    }
}

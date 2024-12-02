#include <atomic>
#include "Runtime/Thread/Processor.h"
#include "Runtime/Thread/Thread.h"
#include "SpinLock.h"

namespace StepEngine
{
    void SpinCounter::Spin()
    {
        if (m_curSpinCount < m_maxSpinCount)
        {
            ++m_curSpinCount;
            PlatformProcessor::Yield();
        }
        else
        {
            m_curSpinCount = 0;
            PlatformThread::Yield();
        }
    }
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

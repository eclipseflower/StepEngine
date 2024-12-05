#include "Runtime/Thread/PlatformThread.h"
#include "SpinCounter.h"
#include "ReentrantLock.h"

namespace StepEngine
{
    bool ReentrantLock::TryLock()
    {
        unsigned int threadId = PlatformThread::GetThreadId();
        bool isLocked = false;
        if(m_threadId.load(std::memory_order_relaxed) == threadId)
        {
            isLocked = true;
        }
        else
        {
            unsigned int expected = 0;
            isLocked = m_threadId.compare_exchange_strong(expected, threadId, std::memory_order_acquire, std::memory_order_relaxed);
        }


        if(isLocked)
        {
            m_refCount.fetch_add(1, std::memory_order_relaxed);
        }
        return isLocked;
    }

    void ReentrantLock::Lock(unsigned int spinCount)
    {
        SpinCounter spinCounter(spinCount);
        unsigned int threadId = PlatformThread::GetThreadId();
        if(m_threadId.load(std::memory_order_relaxed) != threadId)
        {
            unsigned int expected = 0;
            while(!m_threadId.compare_exchange_weak(expected, threadId, std::memory_order_acquire, std::memory_order_relaxed))
            {
                expected = 0;
                spinCounter.Spin();
            }
        }

        m_refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void ReentrantLock::Unlock()
    {
        if(m_refCount.fetch_sub(1, std::memory_order_relaxed) == 1)
        {
            m_threadId.store(0, std::memory_order_release);
        }
    }
}

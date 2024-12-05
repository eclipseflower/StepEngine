#include "Runtime/Thread/PlatformProcessor.h"
#include "Runtime/Thread/PlatformThread.h"
#include "SpinCounter.h"

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
}

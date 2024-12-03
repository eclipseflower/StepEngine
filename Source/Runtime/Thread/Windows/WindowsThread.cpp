#include "WindowsThread.h"

namespace StepEngine
{
    void WindowsThread::Yield()
    {
        SwitchToThread();
    }

    unsigned int WindowsThread::GetThreadId()
    {
        return GetCurrentThreadId();
    }
}

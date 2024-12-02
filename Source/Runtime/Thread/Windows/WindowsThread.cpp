#include "WindowsThread.h"

namespace StepEngine
{
    void WindowsThread::Yield()
    {
        SwitchToThread();
    }
}

#pragma once

#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class WindowsSemaphore
    {
        NON_COPYABLE(WindowsSemaphore);
    public:
        WindowsSemaphore(int initCount, int maxCount);
        ~WindowsSemaphore();
        bool WaitForSignal(int timeoutMs = -1);
        void Signal(int count = 1);
    };
}

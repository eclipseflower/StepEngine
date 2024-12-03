#pragma once

#include <Windows.h>
#include <processthreadsapi.h>

#ifdef Yield
#undef Yield
#endif

namespace StepEngine
{
    class WindowsThread
    {
    public:
        static void Yield();
        static unsigned int GetThreadId();
    };

    using PlatformThread = WindowsThread;
}

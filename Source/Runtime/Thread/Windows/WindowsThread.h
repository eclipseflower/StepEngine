#pragma once

#include <Windows.h>
#ifdef Yield
#undef Yield
#endif

namespace StepEngine
{
    class WindowsThread
    {
    public:
        static void Yield();
    };

    using PlatformThread = WindowsThread;
}

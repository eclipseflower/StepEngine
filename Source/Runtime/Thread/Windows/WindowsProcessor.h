#pragma once

#include <Windows.h>
#ifdef Yield
#undef Yield
#endif

namespace StepEngine
{
    class WindowsProcessor
    {
    public:
        static void Yield();
    };

    using PlatformProcessor = WindowsProcessor;
}

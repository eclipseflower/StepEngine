#pragma once

#include <Windows.h>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class WindowsCriticalSection
    {
        NON_COPYABLE(WindowsCriticalSection);
    public:
        WindowsCriticalSection();
        ~WindowsCriticalSection();
        bool TryLock();
        void Lock();
        void Unlock();
    private:
        CRITICAL_SECTION m_criticalSection;
    };

    using PlatformCriticalSection = WindowsCriticalSection;
}

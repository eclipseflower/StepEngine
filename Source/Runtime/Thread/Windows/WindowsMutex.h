#pragma once

#include <Windows.h>
#include "Core/CoreDefine.h"

namespace StepEngine
{
    class WindowsMutex
    {
        NON_COPYABLE(WindowsMutex);
    public:
        WindowsMutex();
        ~WindowsMutex();
        bool IsValid() const { return m_Mutex != NULL; }
        bool TryLock();
        void Lock();
        void Unlock();
    private:
        HANDLE m_Mutex;
    };
}

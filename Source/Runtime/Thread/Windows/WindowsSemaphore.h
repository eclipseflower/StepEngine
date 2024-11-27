#pragma once

#include <Windows.h>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class WindowsSemaphore
    {
        NON_COPYABLE(WindowsSemaphore);
    public:
        WindowsSemaphore(int initCount, int maxCount);
        ~WindowsSemaphore();
        bool IsValid() const { return m_semaphore != NULL; }
        bool TryAquire(int timeoutMs = 0);
        void Aquire();
        void Release(int count = 1);
    private:
        HANDLE m_semaphore;
    };
}

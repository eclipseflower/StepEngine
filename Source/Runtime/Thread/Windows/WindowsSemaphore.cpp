#include "WindowsSemaphore.h"

namespace StepEngine
{
    WindowsSemaphore::WindowsSemaphore(int initCount, int maxCount)
    {
        m_semaphore = CreateSemaphore(NULL, initCount, maxCount, NULL);
    }
    WindowsSemaphore::~WindowsSemaphore()
    {
        if(!IsValid())
        {
            return;
        }

        CloseHandle(m_semaphore);
        m_semaphore = NULL;
    }
    bool WindowsSemaphore::TryAcquire(int timeoutMs)
    {
        if(!IsValid())
        {
            return false;
        }

        DWORD ret = WaitForSingleObject(m_semaphore, timeoutMs);
        return ret == WAIT_OBJECT_0;
    }
    void WindowsSemaphore::Acquire()
    {
        if(!IsValid())
        {
            return;
        }

        DWORD ret = WaitForSingleObject(m_semaphore, INFINITE);
        if(ret != WAIT_OBJECT_0)
        {
            CloseHandle(m_semaphore);
            m_semaphore = NULL;
        }
    }
    void WindowsSemaphore::Release(int count)
    {
        if(!IsValid())
        {
            return;
        }

        ReleaseSemaphore(m_semaphore, count, NULL);
    }
}

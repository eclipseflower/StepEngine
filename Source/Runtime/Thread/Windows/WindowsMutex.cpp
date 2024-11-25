#include "WindowsMutex.h"

namespace StepEngine
{
    WindowsMutex::WindowsMutex()
    {
        m_Mutex = CreateMutex(NULL, FALSE, NULL);
    }
    WindowsMutex::~WindowsMutex()
    {
        if(!IsValid())
        {
            return;
        }
               
        CloseHandle(m_Mutex);
        m_Mutex = NULL;
    }
    bool WindowsMutex::TryLock()
    {
        if(!IsValid())
        {
            return false;
        }

        DWORD result = WaitForSingleObject(m_Mutex, 0);
        return result == WAIT_OBJECT_0;
    }
    void WindowsMutex::Lock()
    {
        if(!IsValid())
        {
            return;
        }

        DWORD result = WaitForSingleObject(m_Mutex, INFINITE);
        if(result != WAIT_OBJECT_0)
        {
            CloseHandle(m_Mutex);
            m_Mutex = NULL;
        }
    }
    void WindowsMutex::Unlock()
    {
        if(!IsValid())
        {
            return;
        }

        ReleaseMutex(m_Mutex);
    }
}

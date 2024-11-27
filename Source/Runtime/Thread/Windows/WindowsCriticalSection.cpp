#include "WindowsCriticalSection.h"
#include <synchapi.h>

namespace StepEngine
{
    WindowsCriticalSection::WindowsCriticalSection()
    {
        InitializeCriticalSectionAndSpinCount(&m_criticalSection, 400);
    }
    WindowsCriticalSection::~WindowsCriticalSection()
    {
        DeleteCriticalSection(&m_criticalSection);
    }
    bool WindowsCriticalSection::TryLock()
    {
        return TryEnterCriticalSection(&m_criticalSection);
    }
    void WindowsCriticalSection::Lock()
    {
        EnterCriticalSection(&m_criticalSection);
    }
    void WindowsCriticalSection::Unlock()
    {
        LeaveCriticalSection(&m_criticalSection);
    }
}

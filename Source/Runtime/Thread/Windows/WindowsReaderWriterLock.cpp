#include "WindowsReaderWriterLock.h"

namespace StepEngine
{
    WindowsReaderWriterLock::WindowsReaderWriterLock()
    {
        InitializeSRWLock(&m_lock);
    }
    bool WindowsReaderWriterLock::TryReadLock()
    {
        return TryAcquireSRWLockShared(&m_lock);
    }
    void WindowsReaderWriterLock::ReadLock()
    {
        AcquireSRWLockShared(&m_lock);
    }
    void WindowsReaderWriterLock::ReadUnlock()
    {
        ReleaseSRWLockShared(&m_lock);
    }
    bool WindowsReaderWriterLock::TryWriteLock()
    {
        return TryAcquireSRWLockExclusive(&m_lock);
    }
    void WindowsReaderWriterLock::WriteLock()
    {
        AcquireSRWLockExclusive(&m_lock);
    }
    void WindowsReaderWriterLock::WriteUnlock()
    {
        ReleaseSRWLockExclusive(&m_lock);
    }
}

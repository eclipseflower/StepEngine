#pragma once

#include <Windows.h>
#include "Runtime/Core/ClassDefine.h"

namespace StepEngine
{
    class WindowsReaderWriterLock
    {
        NON_COPYABLE(WindowsReaderWriterLock);
    public:
        WindowsReaderWriterLock();
        ~WindowsReaderWriterLock() = default;
        bool TryReadLock();
        void ReadLock();
        void ReadUnlock();
        bool TryWriteLock();
        void WriteLock();
        void WriteUnlock();
    private:
        SRWLOCK m_lock;
    };

    using PlatformReaderWriterLock = WindowsReaderWriterLock;
}

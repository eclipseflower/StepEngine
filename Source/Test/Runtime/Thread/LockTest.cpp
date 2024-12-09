#include <gtest/gtest.h>
#include "Runtime/Thread/PlatformCriticalSection.h"
#include "Runtime/Thread/PlatformMutex.h"
#include "Runtime/Thread/PlatformSemaphore.h"

int gCounter = 0;

StepEngine::PlatformCriticalSection gCriticalSection;
void CriticalSectionFunc() {
    gCriticalSection.Lock();
    for(int i = 0; i < 100; i++) gCounter++;
    std::cout << "gCounter is " << gCounter << std::endl;
    gCriticalSection.Unlock();
}

TEST(LockTest, CriticalSection) {
    std::thread myThreads[5];
    for(int i = 0; i < 5; i++)
    {
        myThreads[i] = std::thread(CriticalSectionFunc);
    }
    for(int i = 0; i < 5; i++)
    {
        myThreads[i].join();
    }
    EXPECT_EQ(gCounter, 500);
}

StepEngine::PlatformMutex gMutex;
void MutexFunc() {
    gMutex.Lock();
    for(int i = 0; i < 100; i++) gCounter++;
    std::cout << "gCounter is " << gCounter << std::endl;
    gMutex.Unlock();
}

TEST(LockTest, Mutex) {
    std::thread myThreads[5];
    for(int i = 0; i < 5; i++)
    {
        myThreads[i] = std::thread(MutexFunc);
    }
    for(int i = 0; i < 5; i++)
    {
        myThreads[i].join();
    }
    EXPECT_EQ(gCounter, 500);
}

StepEngine::PlatformSemaphore gSemaphore(0, 1);

#include <gtest/gtest.h>
#include "Runtime/Thread/PlatformCriticalSection.h"
#include "Runtime/Thread/PlatformMutex.h"
#include "Runtime/Thread/PlatformSemaphore.h"
#include "Runtime/Thread/PlatformReaderWriterLock.h"
#include "Runtime/Thread/ReentrantLock.h"
#include "Runtime/Thread/SpinLock.h"

int gCounter = 0;

StepEngine::PlatformCriticalSection gCriticalSection;
void CriticalSectionFunc() {
    gCriticalSection.Lock();
    for(int i = 0; i < 100; i++) {
        gCounter++;
        Sleep(10);
    }
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
    for(int i = 0; i < 100; i++) { 
        gCounter++;
        Sleep(10);
    }
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

StepEngine::PlatformSemaphore gProducerSemaphore(1, 1);
StepEngine::PlatformSemaphore gConsumerSemaphore(0, 1);
void ProducerFunc() {
    for(int i = 0; i < 100; i++) {
        gProducerSemaphore.Acquire();
        gCounter++;
        Sleep(10);
        gConsumerSemaphore.Release();
    }
}

void ConsumerFunc() {
    for(int i = 0; i < 100; i++) {
        gConsumerSemaphore.Acquire();
        gCounter--;
        Sleep(10);
        gProducerSemaphore.Release();
    }
}

TEST(LockTest, Semaphore) {
    std::thread producerThread(ProducerFunc);
    std::thread consumerThread(ConsumerFunc);
    producerThread.join();
    consumerThread.join();
    EXPECT_EQ(gCounter, 0);
}

StepEngine::PlatformReaderWriterLock gReaderWriterLock;
void ReaderFunc() {
    gReaderWriterLock.ReadLock();
    std::cout << "gCounter is " << gCounter << std::endl;
    Sleep(10);
    gReaderWriterLock.ReadUnlock();
}

void WriteFunc() {
    gReaderWriterLock.WriteLock();
    for(int i = 0; i < 100; i++) {
        gCounter++;
        Sleep(10);
    }
    gReaderWriterLock.WriteUnlock();
}

TEST(LockTest, ReaderWriterLock) {
    std::thread readerThreads[2];
    std::thread writerThreads[2];
    for(int i = 0; i < 2; i++)
    {
        readerThreads[i] = std::thread(ReaderFunc);
        writerThreads[i] = std::thread(WriteFunc);
    }
    for(int i = 0; i < 2; i++)
    {
        readerThreads[i].join();
        writerThreads[i].join();
    }
    EXPECT_EQ(gCounter, 200);
}

StepEngine::SpinLock gSpinLock;
void SpinLockFunc() {
    gSpinLock.Lock();
    for(int i = 0; i < 100; i++) {
        gCounter++;
        Sleep(10);
    }
    std::cout << "gCounter is " << gCounter << std::endl;
    gSpinLock.Unlock();
}

TEST(LockTest, SpinLock) {
    std::thread myThreads[5];
    for(int i = 0; i < 5; i++)
    {
        myThreads[i] = std::thread(SpinLockFunc);
    }
    for(int i = 0; i < 5; i++)
    {
        myThreads[i].join();
    }
    EXPECT_EQ(gCounter, 500);
}

StepEngine::ReentrantLock gReentrantLock;
void ReentrantLockFunc(int maxVal) {
    gReentrantLock.Lock();
    if(gCounter < maxVal)
    {
        gCounter++;
    }
    std::cout << "gCounter is " << gCounter << std::endl;
    Sleep(10);
    if(gCounter < maxVal)
    {
        ReentrantLockFunc(maxVal);
    }
    gReentrantLock.Unlock();
}

TEST(LockTest, ReentrantLock) {
    int threadCount = 5;
    std::thread myThreads[threadCount];
    for(int i = 0; i < threadCount; i++)
    {
        myThreads[i] = std::thread(ReentrantLockFunc, 100 * threadCount);
    }
    for(int i = 0; i < threadCount; i++)
    {
        myThreads[i].join();
    }
    EXPECT_EQ(gCounter, 500);
}

void NoLockFunc() {
    for(int i = 0; i < 100; i++) {
        gCounter++;
        Sleep(10);
    }
    std::cout << "gCounter is " << gCounter << std::endl;
}

TEST(LockTest, NoLock) {
    std::thread myThreads[5];
    for(int i = 0; i < 5; i++)
    {
        myThreads[i] = std::thread(NoLockFunc);
    }
    for(int i = 0; i < 5; i++)
    {
        myThreads[i].join();
    }
    EXPECT_NE(gCounter, 500);
}

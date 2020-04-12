#include "EngineTimer.h"

Engine::EngineTimer::EngineTimer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
#endif
}

float Engine::EngineTimer::TotalTime() const
{
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

float Engine::EngineTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void Engine::EngineTimer::Reset()
{
	__int64 currTime;
#ifdef ENGINE_PLATFORM_DIRECTX
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Engine::EngineTimer::Start()
{
	__int64 startTime;

	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void Engine::EngineTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
#ifdef ENGINE_PLATFORM_DIRECTX
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif

		mStopTime = currTime;
		mStopped = true;
	}
}

void Engine::EngineTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
#ifdef ENGINE_PLATFORM_DIRECTX
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
#endif
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
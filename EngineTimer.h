#ifndef __ENGINE_TIMER_H__
#define __ENGINE_TIMER_H__

#include "EngineUtil.h"

namespace Engine
{
	class EngineTimer
	{
	public:
		EngineTimer();

		float TotalTime()const;  // in seconds
		float DeltaTime()const; // in seconds
		float FPS()const;

		void Reset(); // Call before message loop.
		void Start(); // Call when unpaused.
		void Stop();  // Call when paused.
		void Tick();  // Call every frame.

	private:
		double mSecondsPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;

		bool mStopped;
	};
}

#endif // __ENGINE_TIMER_H__

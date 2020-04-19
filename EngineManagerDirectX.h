#ifndef __ENGINE_MANAGER_H__
#define __ENGINE_MANAGER_H__

#include "EngineWindowDirectX.h"
#include "EngineCoreDirectX.h"
#include "EngineTimer.h"

using Engine::Window::EngineWindowDirectX;
using Engine::Core::EngineCoreDirectX;

namespace Engine
{
	class EngineManagerDirectX
	{
	public:
		EngineManagerDirectX();
		~EngineManagerDirectX();
		bool InitEngineWindow(HINSTANCE hInstance);
		bool InitEngineCore(bool enableMsaa, UINT msaaCount);
		int RunEngine();
		void EngnineLoop(bool paused);
		void OnPause(bool paused);
		void OnResize();
		UINT GetWindowWidth();
		UINT GetWindowHeight();
		HWND GetHwnd();

		EngineWindowDirectX *mWindowInst;
		EngineCoreDirectX *mCoreInst;
		EngineTimer mTimerInst;
	};

	extern EngineManagerDirectX * gManagerDirectX;
}

#endif // __ENGINE_MANAGER_H__

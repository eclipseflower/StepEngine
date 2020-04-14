#ifndef __ENGINE_MANAGER_H__
#define __ENGINE_MANAGER_H__

#include "EngineWindowDirectX.h"
#include "EngineTimer.h"

using Engine::Window::EngineWindowDirectX;

namespace Engine
{
	class EngineManagerDirectX
	{
	public:
		EngineManagerDirectX();
		~EngineManagerDirectX();
		bool InitEngineWindow(HINSTANCE hInstance);

		EngineWindowDirectX *mWindowInst;
		EngineTimer mTimerInst;
	};

	EngineManagerDirectX * gManagerDirectX = nullptr;
}

#endif // __ENGINE_MANAGER_H__

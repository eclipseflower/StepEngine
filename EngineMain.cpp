#ifdef ENGINE_PLATFORM_DIRECTX

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "EngineWindowDirectX.h"

using Engine::Window::EngineWindowDirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	EngineWindowDirectX window;
	if (!window.Init())
	{
		return -1;
	}
}

#endif // ENGINE_PLATFORM_DIRECTX
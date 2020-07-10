#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "EngineBoxApp.h"

using Engine::DxException;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		EngineBoxApp app;
		return app.Run(hInstance);
	}
	catch (DxException& e)
	{
		MessageBoxW(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}
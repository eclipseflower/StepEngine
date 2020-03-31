#include "EngineLog.h"

void Engine::Debug::LogMessageBox(const char *msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_INFO, MB_ICONINFORMATION);
#endif
}

void Engine::Debug::LogWarningMessageBox(const char *msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_WARNING, MB_ICONWARNING);
#endif
}

void Engine::Debug::LogErrorMessageBox(const char *msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_ERROR, MB_ICONERROR);
#endif
}

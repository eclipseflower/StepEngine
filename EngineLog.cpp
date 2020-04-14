#include "EngineLog.h"

const char *Engine::Debug::EngineLog::DEBUG_INFO = "INFO";
const char *Engine::Debug::EngineLog::DEBUG_WARNING = "INFO";
const char *Engine::Debug::EngineLog::DEBUG_ERROR = "INFO";

void Engine::Debug::EngineLog::LogMessageBox(const char * msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_INFO, MB_ICONINFORMATION);
#endif
}

void Engine::Debug::EngineLog::LogWarningMessageBox(const char * msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_WARNING, MB_ICONWARNING);
#endif
}

void Engine::Debug::EngineLog::LogErrorMessageBox(const char * msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg, DEBUG_ERROR, MB_ICONERROR);
#endif
}

#include "EngineLog.h"

const string Engine::Debug::EngineLog::DEBUG_INFO = "INFO";
const string Engine::Debug::EngineLog::DEBUG_WARNING = "INFO";
const string Engine::Debug::EngineLog::DEBUG_ERROR = "INFO";

void Engine::Debug::EngineLog::LogMessageBox(const string msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg.c_str(), DEBUG_INFO.c_str(), MB_ICONINFORMATION);
#endif
}

void Engine::Debug::EngineLog::LogWarningMessageBox(const string msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg.c_str(), DEBUG_WARNING.c_str(), MB_ICONWARNING);
#endif
}

void Engine::Debug::EngineLog::LogErrorMessageBox(const string msg)
{
#ifdef ENGINE_PLATFORM_DIRECTX
	MessageBox(nullptr, msg.c_str(), DEBUG_ERROR.c_str(), MB_ICONERROR);
#endif
}

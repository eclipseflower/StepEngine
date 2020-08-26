#include "EngineLog.h"

const string Engine::Debug::EngineLog::DEBUG_INFO = "INFO";
const string Engine::Debug::EngineLog::DEBUG_WARNING = "INFO";
const string Engine::Debug::EngineLog::DEBUG_ERROR = "INFO";

void Engine::Debug::EngineLog::LogMessageBox(const string msg)
{
	MessageBox(nullptr, msg.c_str(), DEBUG_INFO.c_str(), MB_ICONINFORMATION);
}

void Engine::Debug::EngineLog::LogWarningMessageBox(const string msg)
{
	MessageBox(nullptr, msg.c_str(), DEBUG_WARNING.c_str(), MB_ICONWARNING);
}

void Engine::Debug::EngineLog::LogErrorMessageBox(const string msg)
{
	MessageBox(nullptr, msg.c_str(), DEBUG_ERROR.c_str(), MB_ICONERROR);
}

void Engine::Debug::EngineLog::LogDebugString(const string msg)
{
	OutputDebugString(msg.c_str());
}

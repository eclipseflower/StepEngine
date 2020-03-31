#ifndef __ENGINE_LOG_H__
#define __ENGINE_LOG_H__

#include "EngineUtil.h"

using std::string;

namespace Engine
{
	namespace Debug
	{
		const char *DEBUG_INFO = "INFO";
		const char *DEBUG_WARNING = "WARNING";
		const char *DEBUG_ERROR = "ERROR";

		void LogMessageBox(const char *msg);
		void LogWarningMessageBox(const char *msg);
		void LogErrorMessageBox(const char *msg);
	}
}

#endif // __ENGINE_LOG_H__
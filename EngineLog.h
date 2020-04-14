#ifndef __ENGINE_LOG_H__
#define __ENGINE_LOG_H__

#include "EngineUtil.h"

using std::string;

namespace Engine
{
	namespace Debug
	{
		class EngineLog
		{
		public:
			static const char *DEBUG_INFO;
			static const char *DEBUG_WARNING;
			static const char *DEBUG_ERROR;

			static void LogMessageBox(const char *msg);
			static void LogWarningMessageBox(const char *msg);
			static void LogErrorMessageBox(const char *msg);
		};
	}
}

#endif // __ENGINE_LOG_H__
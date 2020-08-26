#ifndef __ENGINE_LOG_H__
#define __ENGINE_LOG_H__

#include "EngineUtilDirectX.h"

using std::string;

namespace Engine
{
	namespace Debug
	{
		class EngineLog
		{
		public:
			static const string DEBUG_INFO;
			static const string DEBUG_WARNING;
			static const string DEBUG_ERROR;

			static void LogMessageBox(const string msg);
			static void LogWarningMessageBox(const string msg);
			static void LogErrorMessageBox(const string msg);
			static void LogDebugString(const string msg);
		};
	}
}

#endif // __ENGINE_LOG_H__
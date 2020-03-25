#ifndef __ENGINE_WINDOW_H__
#define __ENGINE_WINDOW_H__

#include "EngineUtil.h"

using std::string;

namespace Engine
{
	namespace Window
	{
		class EngineWindow
		{
		public:
			EngineWindow();
			~EngineWindow();
			virtual bool Init();

		protected:
			string mCaption;
			int mWidth;
			int mHeight;
			bool mMinimized;
			bool mMaximized;
			bool mResizing;
		};
	}
}

#endif // __ENGINE_WINDOW_H__
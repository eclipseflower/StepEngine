#ifndef __ENGINE_WINDOW_H__
#define __ENGINE_WINDOW_H__

#include "EngineUtil.h"

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
			char *mCaption;
			int mWidth;
			int mHeight;
			bool mMinimized;
			bool mMaximized;
			bool mResizing;
			bool mPaused;
		};
	}
}

#endif // __ENGINE_WINDOW_H__
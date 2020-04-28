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
			virtual void SetCaption(string caption);

			UINT GetWidth();
			UINT GetHeight();
			string GetCaption() const;

		protected:
			string mCaption = "StepEngine";
			UINT mWidth = 800;
			UINT mHeight = 600;
			bool mMinimized = false;
			bool mMaximized = false;
			bool mResizing = false;
			bool mPaused = false;
		};
	}
}

#endif // __ENGINE_WINDOW_H__
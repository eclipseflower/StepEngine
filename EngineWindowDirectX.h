#ifndef __ENGINE_WINDOW_DIRECTX_H__
#define __ENGINE_WINDOW_DIRECTX_H__

#include "EngineUtilDirectX.h"
#include "EngineWindow.h"

namespace Engine
{
	namespace Window
	{
		class EngineWindowDirectX : public EngineWindow
		{
		public:
			EngineWindowDirectX();
			~EngineWindowDirectX();

		private:
			HINSTANCE mHAppInst;
			HWND mHMainWnd;
		};
	}
}

#endif // !__ENGINE_WINDOW_DIRECTX_H__
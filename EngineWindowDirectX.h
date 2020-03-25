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

			bool Init() override;

		private:
			HINSTANCE mhAppInst;
			HWND mhMainWnd;

			const char mWndClassName[] = "EngineWindowDirectX";

			static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};
	}
}

#endif // !__ENGINE_WINDOW_DIRECTX_H__
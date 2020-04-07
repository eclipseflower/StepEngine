#ifndef __ENGINE_WINDOW_DIRECTX_H__
#define __ENGINE_WINDOW_DIRECTX_H__

#include "EngineWindow.h"
#include "EngineLog.h"
#include "EngineGlobalDirectX.h"

using Engine::Debug;

namespace Engine
{
	namespace Window
	{
		class EngineWindowDirectX : public EngineWindow
		{
		public:
			EngineWindowDirectX(HINSTANCE hInstance);
			~EngineWindowDirectX();

			bool Init() override;

		private:
			HINSTANCE mhAppInst;
			HWND mhMainWnd;

			const char mWndClassName[] = "EngineWindowDirectX";

			LRESULT CALLBACK InnerWindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};

		LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	}
}

#endif // !__ENGINE_WINDOW_DIRECTX_H__
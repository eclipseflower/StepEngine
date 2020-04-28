#ifndef __ENGINE_WINDOW_DIRECTX_H__
#define __ENGINE_WINDOW_DIRECTX_H__

#include "EngineWindow.h"
#include "EngineLog.h"

using Engine::Debug::EngineLog;

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
			void SetCaption(string caption) override;
			LRESULT CALLBACK EngineWindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			HWND GetHandle();
			int WindowLoop();

		private:
			HINSTANCE mhAppInst;
			HWND mhMainWnd;

			const string mWndClassName = "EngineWindowDirectX";
		};

		LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	}
}

#endif // !__ENGINE_WINDOW_DIRECTX_H__
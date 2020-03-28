#include "EngineWindowDirectX.h"

Engine::Window::EngineWindowDirectX::EngineWindowDirectX()
{
	gWindowDirectX = this;
}

Engine::Window::EngineWindowDirectX::~EngineWindowDirectX()
{
}

bool Engine::Window::EngineWindowDirectX::Init()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = mWndClassName;
}

LRESULT Engine::Window::EngineWindowDirectX::InnerWindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mPaused = true;
		}
		else
		{
			mPaused = false;
		}
		return 0;
	}
}

LRESULT Engine::Window::WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gWindowDirectX != nullptr)
	{
		return gWindowDirectX->InnerWindowProcess(hwnd, msg, wParam, lParam);
	}
}

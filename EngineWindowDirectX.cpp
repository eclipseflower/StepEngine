#include "EngineWindowDirectX.h"
#include "EngineManagerDirectX.h"

Engine::Window::EngineWindowDirectX::EngineWindowDirectX(HINSTANCE hInstance)
{
	mhAppInst = hInstance;
	gManagerDirectX->mWindowInst = this;
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

	if (!RegisterClass(&wc))
	{
		EngineLog::LogErrorMessageBox("RegisterClass Failed");
		return false;
	}

	RECT rect = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	mhMainWnd = CreateWindow(mWndClassName, mCaption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, mhAppInst, 0);

	if (!mhMainWnd)
	{
		EngineLog::LogErrorMessageBox("CreateWindow Failed");
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

LRESULT Engine::Window::EngineWindowDirectX::EngineWindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mPaused = true;
			gManagerDirectX->OnPause(mPaused);
		}
		else
		{
			mPaused = false;
			gManagerDirectX->OnPause(mPaused);
		}
		return 0;

	case WM_SIZE:
		mWidth = LOWORD(lParam);
		mHeight = HIWORD(lParam);

		if (wParam == SIZE_MINIMIZED)
		{
			mPaused = true;
			mMinimized = true;
			mMaximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			mPaused = false;
			mMinimized = false;
			mMaximized = true;
			gManagerDirectX->OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (mMinimized)
			{
				mPaused = false;
				mMinimized = false;
				gManagerDirectX->OnResize();
			}
			else if (mMaximized)
			{
				mPaused = false;
				mMaximized = false;
				gManagerDirectX->OnResize();
			}
			else if (mResizing)
			{

			}
			else
			{
				gManagerDirectX->OnResize();
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		mPaused = true;
		mResizing = true;
		gManagerDirectX->OnPause(mPaused);
		return 0;

	case WM_EXITSIZEMOVE:
		mPaused = false;
		mResizing = false;
		gManagerDirectX->OnPause(mPaused);
		gManagerDirectX->OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		return 0;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		return 0;

	case WM_MOUSEMOVE:
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND Engine::Window::EngineWindowDirectX::GetHandle()
{
	return mhMainWnd;
}

int Engine::Window::EngineWindowDirectX::WindowLoop()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			gManagerDirectX->EngnineLoop(mPaused);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK Engine::Window::WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gManagerDirectX->mWindowInst != nullptr)
	{
		return gManagerDirectX->mWindowInst->EngineWindowProcess(hwnd, msg, wParam, lParam);
	}
	return 0;
}

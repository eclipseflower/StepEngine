#include "EngineWindowDirectX.h"

Engine::Window::EngineWindowDirectX::EngineWindowDirectX(HINSTANCE hInstance)
{
	mhAppInst = hInstance;
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

	if (!RegisterClass(&wc))
	{
		Debug::LogErrorMessageBox("RegisterClass Failed");
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
		Debug::LogErrorMessageBox("CreateWindow Failed");
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
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
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (mMinimized)
			{
				mPaused = false;
				mMinimized = false;
			}
			else if (mMaximized)
			{
				mPaused = false;
				mMaximized = false;
			}
			else if (mResizing)
			{

			}
			else
			{

			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		mPaused = true;
		mResizing = true;
		return 0;

	case WM_EXITSIZEMOVE:
		mPaused = false;
		mResizing = false;
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

LRESULT CALLBACK Engine::Window::WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gWindowDirectX != nullptr)
	{
		return gWindowDirectX->InnerWindowProcess(hwnd, msg, wParam, lParam);
	}
	return 0;
}

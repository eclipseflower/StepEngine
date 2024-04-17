#include "WindowsApplication.h"
#include "WindowsWindow.h"
#include "Core/CoreGlobals.h"

namespace Step
{
    WindowsApplication* g_pWindowsApplication = nullptr;

    GenericApplication* WindowsApplication::CreateApplication()
    {
        return WindowsApplication::CreateWindowsApplication(g_hInstance);
    }

    WindowsApplication* WindowsApplication::CreateWindowsApplication(const HINSTANCE hInstance)
    {
        g_pWindowsApplication = new WindowsApplication(hInstance);
        return g_pWindowsApplication;
    }

    void WindowsApplication::PumpMessages()
    {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GenericWindow* WindowsApplication::MakeWindow()
    {
        return new WindowsWindow();
    }

    void WindowsApplication::InitializeWindow(GenericWindow* pWindow, GenericWindowDefinition* pDefinition)
    {
        WindowsWindow* pWindowsWindow = static_cast<WindowsWindow*>(pWindow);
        pWindowsWindow->Initialize(pDefinition, m_hInstance);
    }

    LRESULT CALLBACK WindowsApplication::AppWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return g_pWindowsApplication->ProcessMessage(hWnd, msg, wParam, lParam);
    }

    WindowsApplication::WindowsApplication(const HINSTANCE hInstance)
    : m_hInstance(hInstance)
    {
        RegisterApplicationClass(hInstance);
    }

    bool WindowsApplication::RegisterApplicationClass(const HINSTANCE hInstance)
    {
        WNDCLASSEX wcex;
        ZeroMemory(&wcex, sizeof(WNDCLASSEX));

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = AppWndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = APP_WINDOW_CLASS;

        if (!RegisterClassEx(&wcex))
        {
            return false;
        }

        return true;
    }

    int WindowsApplication::ProcessMessage(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            RequestEngineExit();
            return 0;

        default:
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
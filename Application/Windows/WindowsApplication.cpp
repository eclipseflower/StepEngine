#include "WindowsApplication.h"

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

    LRESULT CALLBACK WindowsApplication::AppWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {

    }

    WindowsApplication::WindowsApplication(const HINSTANCE hInstance)
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
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"StepEngine";
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassEx(&wcex))
        {
            return false;
        }

        return true;
    }
}
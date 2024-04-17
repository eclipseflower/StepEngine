#include "WindowsWindow.h"

namespace Step
{
    void WindowsWindow::Initialize(GenericWindowDefinition *pDefinition, HINSTANCE hInstance)
    {
        m_hWnd = CreateWindowEx(
            0,
            APP_WINDOW_CLASS,
            pDefinition->pTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            pDefinition->windowWidth,
            pDefinition->windowHeight,
            NULL,
            NULL,
            hInstance,
            NULL);
    }
}
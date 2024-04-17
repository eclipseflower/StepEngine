#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "Generic/GenericWindow.h"
#include "Generic/GenericWindowDefinition.h"
#include <Windows.h>

namespace Step
{
    static constexpr wchar_t APP_WINDOW_CLASS[] = L"StepEngine";

    class WindowsWindow : public GenericWindow
    {
    public:
        void Initialize(GenericWindowDefinition *pDefinition, HINSTANCE hInstance);

    private:
        HWND m_hWnd;
    };
}

#endif // WINDOWS_WINDOW_H
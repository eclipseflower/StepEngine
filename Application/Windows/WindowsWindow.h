#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "Application/Generic/GenericWindow.h"
#include "Application/Generic/GenericWindowDefinition.h"
#include <Windows.h>

namespace Step
{
    static constexpr char APP_WINDOW_CLASS[] = "StepEngine";

    class WindowsWindow : public GenericWindow
    {
    public:
        void Initialize(GenericWindowDefinition *pDefinition, HINSTANCE hInstance);
        virtual void Show() override;

    private:
        HWND m_hWnd;
    };
}

#endif // WINDOWS_WINDOW_H
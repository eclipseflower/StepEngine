#ifndef WINDOWS_APPLICATION_H
#define WINDOWS_APPLICATION_H

#include "WindowsPlatform.h"
#include "Generic/GenericApplication.h"
#include <cstdint>

namespace Step
{
    class WindowsApplication : public GenericApplication
    {
    public:
        static GenericApplication* CreateApplication();
        static WindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance);

        virtual GenericWindow* MakeWindow() override;
        virtual void InitializeWindow(GenericWindow* pWindow) override;

    protected:
        static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        WindowsApplication(const HINSTANCE hInstance);
        bool RegisterApplicationClass(const HINSTANCE hInstance);
        int ProcessMessage(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif // WINDOWS_APPLICATION_H
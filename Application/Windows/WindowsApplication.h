#ifndef WINDOWS_APPLICATION_H
#define WINDOWS_APPLICATION_H

#include "WindowsPlatform.h"
#include "Generic/GenericApplication.h"

namespace Step
{
    class WindowsApplication : public GenericApplication
    {
    public:
        static GenericApplication* CreateApplication();
        static WindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance);

    protected:
        static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        WindowsApplication(const HINSTANCE hInstance);
        bool RegisterApplicationClass(const HINSTANCE hInstance);
    };
}

#endif // WINDOWS_APPLICATION_H
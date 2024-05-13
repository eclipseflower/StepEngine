#ifndef WINDOWS_APPLICATION_H
#define WINDOWS_APPLICATION_H

#include "Core/Windows/WindowsPlatform.h"
#include "Application/Generic/GenericApplication.h"
#include <cstdint>

namespace Step
{
    class WindowsApplication : public GenericApplication
    {
    public:
        static GenericApplication* CreateApplication();
        static WindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance);
        static void PumpMessages();

        virtual GenericWindow* MakeWindow() override;
        virtual void InitializeWindow(GenericWindow* pWindow, GenericWindowDefinition* pDefinition) override;

    protected:
        static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        WindowsApplication(const HINSTANCE hInstance);
        bool RegisterApplicationClass(const HINSTANCE hInstance);
        int ProcessMessage(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

    private:
        HINSTANCE m_hInstance;
    };

    using PlatformApplication = WindowsApplication;
}

#endif // WINDOWS_APPLICATION_H
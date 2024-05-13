#include "EngineLoop.h"
#include "Application/Windows/WindowsApplication.h"

namespace Step
{
    GenericApplication* g_pPlatformApplication = nullptr;

    int EngineLoop::PreInit()
    {
        g_pPlatformApplication = PlatformApplication::CreateApplication();
        return 0;
    }

    int EngineLoop::Init()
    {
        GenericWindow* pWindow = g_pPlatformApplication->MakeWindow();
        GenericWindowDefinition definition = { "StepEngine", 960, 540 };
        g_pPlatformApplication->InitializeWindow(pWindow, &definition);
        pWindow->Show();
        return 0;
    }

    void EngineLoop::Tick()
    {
        PlatformApplication::PumpMessages();
    }
}
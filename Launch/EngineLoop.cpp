#include "EngineLoop.h"
#include "WindowsApplication.h"

namespace Step
{
    GenericApplication* g_pPlatformApplication = nullptr;

    int EngineLoop::PreInit()
    {
        g_pPlatformApplication = WindowsApplication::CreateApplication();
        return 0;
    }

    virtual int EngineLoop::Init()
    {
        GenericWindow* pWindow = g_pPlatformApplication->MakeWindow();
        g_pPlatformApplication->InitializeWindow(pWindow);
        return 0;
    }
}
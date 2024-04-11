#include "EngineLoop.h"
#include "WindowsApplication.h"

namespace Step
{
    int EngineLoop::PreInit()
    {
        WindowsApplication::CreateApplication();
        return 0;
    }
}
#include "Core/CoreGlobals.h"
#include "EngineLoop.h"

namespace Step
{
    EngineLoop g_EngineLoop;
    int EnginePreInit()
    {
        int error = g_EngineLoop.PreInit();
        return error;
    }

    int EngineInit()
    {
        int error = g_EngineLoop.Init();
        return error;
    }

    void EngineTick(void)
    {
        g_EngineLoop.Tick();
    }

    void EngineExit()
    {
        RequestEngineExit();
        g_EngineLoop.Exit();
    }

    int LaunchMain()
    {
        int error = EnginePreInit();
        if(error != 0 || IsEngineExitRequested())
        {
            return error;
        }

        error = EngineInit();

        while(!IsEngineExitRequested())
		{
			EngineTick();
		}

        EngineExit();

        return error;
    }
}
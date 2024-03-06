#include "CoreGlobals.h"

namespace Step
{
    bool g_bRequestingExit = false;

    void RequestEngineExit()
    {
        g_bRequestingExit = true;
    }
}
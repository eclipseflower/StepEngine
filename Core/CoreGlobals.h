#ifndef _CORE_GLOBALS_H_
#define _CORE_GLOBALS_H_

namespace Step
{
    extern bool g_bRequestingExit;
    __forceinline bool IsEngineExitRequested()
    {
        return g_bRequestingExit;
    }
}

#endif // _CORE_GLOBALS_H_
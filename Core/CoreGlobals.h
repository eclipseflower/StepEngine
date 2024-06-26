#ifndef _CORE_GLOBALS_H_
#define _CORE_GLOBALS_H_

namespace Step
{
    extern bool g_bRequestingExit;
    inline bool IsEngineExitRequested()
    {
        return g_bRequestingExit;
    }

    void RequestEngineExit();
}

#endif // _CORE_GLOBALS_H_
#ifndef _ENGINE_LOOP_H_
#define _ENGINE_LOOP_H_

#include "Engine/StepEngine.h"

namespace Step
{
    class EngineLoop : public IEngineLoop
    {
    public:
        virtual ~EngineLoop() {}
        int PreInit();
        virtual int Init();
        virtual void Tick() {}
        void Exit() {}
    };

    extern EngineLoop g_EngineLoop;
}

#endif // _ENGINE_LOOP_H_
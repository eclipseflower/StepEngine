#ifndef _STEP_ENGINE_H_
#define _STEP_ENGINE_H_

namespace Step
{
    class IEngineLoop
    {
    public:
        virtual int Init() = 0;
        virtual void Tick() = 0;
    };
}

#endif // _STEP_ENGINE_H_
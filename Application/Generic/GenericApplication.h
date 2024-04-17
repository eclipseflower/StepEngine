#ifndef GENERIC_APPLICATION_H
#define GENERIC_APPLICATION_H

#include "GenericWindow.h"
#include "GenericWindowDefinition.h"

namespace Step
{
    class GenericApplication
    {
    public:
        virtual GenericWindow* MakeWindow() { return new GenericWindow; }
        virtual void InitializeWindow(GenericWindow* pWindow, GenericWindowDefinition* pDefinition) {}
    };
}

#endif // GENERIC_APPLICATION_H
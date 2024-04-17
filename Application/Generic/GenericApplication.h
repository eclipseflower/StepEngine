#ifndef GENERIC_APPLICATION_H
#define GENERIC_APPLICATION_H

#include "GenericWindow.h"

namespace Step
{
    class GenericApplication
    {
    public:
        virtual GenericWindow* MakeWindow();
        virtual void InitializeWindow(GenericWindow* pWindow);
    };
}

#endif // GENERIC_APPLICATION_H
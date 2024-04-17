#ifndef _GENERIC_WINDOW_DEFINITION_H_
#define _GENERIC_WINDOW_DEFINITION_H_

#include <cstdint>

namespace Step
{
    struct GenericWindowDefinition
    {
        const char* pTitle;
        uint32_t windowWidth;
        uint32_t windowHeight;
    };
}

#endif // _GENERIC_WINDOW_DEFINITION_H_
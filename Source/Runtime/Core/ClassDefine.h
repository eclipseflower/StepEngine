#pragma once

#define NON_COPYABLE(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

#pragma once

#define STEP_ENGINE_PLATFORM Windows

#define PLATFORM_HEADER(prefix, name) prefix##STEP_ENGINE_PLATFORM/STEP_ENGINE_PLATFORM##name.h

#pragma once

#include "../Math/Integer.h"

#define QUANTA_OPERATING_SYSTEM_UNKNOWN 0
#define QUANTA_OPERATING_SYSTEM_WINDOWS 1
#define QUANTA_OPERATING_SYSTEM_OSX 2
#define QUANTA_OPERATING_SYSTEM_GNU_LINUX 3

#if defined(_WIN32) || defined(_WIN64) 
#define QUANTA_OPERATING_SYSTEM QUANTA_OPERATING_SYSTEM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define QUANTA_OPERATING_SYSTEM QUANTA_OPERATING_SYSTEM_OSX
#elif defined(__linux__)
#define QUANTA_OPERATING_SYSTEM QUANTA_OPERATING_SYSTEM_GNU_LINUX
#else
#define QUANTA_OPERATING_SYSTEM QUANTA_OPERATING_SYSTEM_UNKNOWN
#endif

namespace Quanta
{
    enum class OperatingSystem : UInt8
    {
        Unknown = QUANTA_OPERATING_SYSTEM_UNKNOWN,
        Windows = QUANTA_OPERATING_SYSTEM_WINDOWS,
        Osx = QUANTA_OPERATING_SYSTEM_OSX,
        GnuLinux = QUANTA_OPERATING_SYSTEM_GNU_LINUX
    };

    constexpr OperatingSystem GetOperatingSystem()
    {
        return static_cast<OperatingSystem>(QUANTA_OPERATING_SYSTEM);
    }
}
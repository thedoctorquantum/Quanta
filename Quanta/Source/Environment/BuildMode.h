#pragma once

#include <Quanta/Math/Integer.h>

#define BUILD_MODE_UNKNOWN 0
#define BUILD_MODE_DEBUG 1
#define BUILD_MODE_RELEASE 2

#ifndef NDEBUG
#define BUILD_MODE BUILD_MODE_DEBUG
#else
#define BUILD_MODE BUILD_MODE_RELEASE
#endif

namespace Quanta
{
    enum class BuildMode : U8
    {
        Unknown = BUILD_MODE_UNKNOWN,
        Debug = BUILD_MODE_DEBUG,
        Release = BUILD_MODE_RELEASE
    };
    
    constexpr BuildMode GetBuildMode() 
    {
        return static_cast<BuildMode>(BUILD_MODE);
    }
}
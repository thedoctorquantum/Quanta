#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class FilterMode : UInt8
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapLinear,
        NearestMipMapLinear,
        LinearMipMapNearest
    };
}
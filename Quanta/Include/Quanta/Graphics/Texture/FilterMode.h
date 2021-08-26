#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class FilterMode : U8
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapLinear,
        NearestMipMapLinear,
        LinearMipMapNearest
    };
}
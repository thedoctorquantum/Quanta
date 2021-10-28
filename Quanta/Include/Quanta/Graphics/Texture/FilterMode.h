#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct FilterMode : U8
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapLinear,
        NearestMipMapLinear,
        LinearMipMapNearest
    };
}
#pragma once

#include <cstdint>

namespace Quanta
{
    enum class FilterMode : uint8_t
    {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapLinear,
        NearestMipMapLinear,
        LinearMipMapNearest
    };
}
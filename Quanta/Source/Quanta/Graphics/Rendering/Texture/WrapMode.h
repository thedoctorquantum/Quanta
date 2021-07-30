#pragma once

#include <cstdint>

namespace Quanta
{
    enum class WrapMode : uint8_t
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
}
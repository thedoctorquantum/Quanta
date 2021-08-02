#pragma once

#include <cstdint>

namespace Quanta
{
    enum class BlendMode : uint8_t
    {
        None,
        Add,
        Subtract,
        ReverseSubtract
    };
}
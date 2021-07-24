#pragma once

#include <stdint.h>

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
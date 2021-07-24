#pragma once

#include <stdint.h>

namespace Quanta
{
    enum class DepthTestMode : uint8_t
    {
        None,
        Always,
        Never,
        Equal,
        NotEqual,
        Greater,
        GreaterOrEqual,
        Less,
        LessOrEqual
    };
}
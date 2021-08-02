#pragma once

#include <cstdint>

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
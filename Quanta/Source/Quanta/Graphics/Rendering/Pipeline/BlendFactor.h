#pragma once

#include <stdint.h>

namespace Quanta
{
    enum class BlendFactor : uint8_t
    {
        Zero,
        One,
        SourceColor,
        InverseSourceColor,
        SourceAlpha,
        InverseSourceAlpha
    };
}
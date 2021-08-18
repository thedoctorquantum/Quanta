#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class BlendFactor : UInt8
    {
        Zero,
        One,
        SourceColor,
        InverseSourceColor,
        SourceAlpha,
        InverseSourceAlpha
    };
}
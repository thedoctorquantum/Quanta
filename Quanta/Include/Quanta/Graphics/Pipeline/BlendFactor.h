#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class BlendFactor : U8
    {
        Zero,
        One,
        SourceColor,
        InverseSourceColor,
        SourceAlpha,
        InverseSourceAlpha
    };
}
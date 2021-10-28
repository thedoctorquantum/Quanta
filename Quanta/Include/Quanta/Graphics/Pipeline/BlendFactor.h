#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct BlendFactor : U8
    {
        Zero,
        One,
        SourceColor,
        InverseSourceColor,
        SourceAlpha,
        InverseSourceAlpha
    };
}
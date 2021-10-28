#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct DepthTestMode : U8
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
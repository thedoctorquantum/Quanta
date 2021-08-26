#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class DepthTestMode : U8
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
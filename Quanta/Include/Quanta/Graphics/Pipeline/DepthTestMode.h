#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class DepthTestMode : UInt8
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
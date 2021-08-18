#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class WrapMode : UInt8
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
}
#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class WrapMode : U8
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
}
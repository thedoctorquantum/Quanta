#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct WrapMode : U8
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
}
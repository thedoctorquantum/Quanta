#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct TexelFormat : U8
    {
        Rgba8I,
        Rgba16F,
        Rgba32F,
        R32I,
        Depth24Stencil8
    };
}
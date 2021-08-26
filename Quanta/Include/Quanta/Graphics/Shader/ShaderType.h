#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class ShaderType : U8
    {
        Vertex,
        Pixel,
        Geometry,
        Compute
    };
}
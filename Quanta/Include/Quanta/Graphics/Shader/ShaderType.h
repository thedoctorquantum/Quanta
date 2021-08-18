#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum class ShaderType : UInt8
    {
        Vertex,
        Pixel,
        Geometry,
        Compute
    };
}
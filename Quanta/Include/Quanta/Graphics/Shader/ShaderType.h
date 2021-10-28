#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    enum struct ShaderType : U8
    {
        Vertex,
        Pixel,
        Geometry,
        Compute
    };
}
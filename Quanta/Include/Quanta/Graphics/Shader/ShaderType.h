#pragma once

#include <cstdint>

namespace Quanta
{
    enum class ShaderType : uint8_t
    {
        Vertex,
        Pixel,
        Geometry,
        Compute
    };
}
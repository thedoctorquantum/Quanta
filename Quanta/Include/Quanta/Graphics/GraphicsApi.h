#pragma once

#include <cstdint>

namespace Quanta
{
    enum class GraphicsApi : uint8_t 
    {
        OpenGL,
        Vulkan,
        Direct3D11,
        Metal
    };
}
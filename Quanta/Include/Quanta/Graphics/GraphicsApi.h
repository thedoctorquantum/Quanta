#pragma once

#include <cstdint>

namespace Quanta
{
    enum struct GraphicsApi : uint8_t 
    {
        OpenGL,
        Vulkan,
        Direct3D11,
        Metal
    };
}
#pragma once

#include <cstdint>

namespace Quanta
{
    enum class TextureType : uint8_t
    {
        Texture1D,
        Texture2D,
        Texture3D,
        CubeMap
    };
}
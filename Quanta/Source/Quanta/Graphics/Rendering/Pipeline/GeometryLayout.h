#pragma once

#include <cstdint>

namespace Quanta
{
    enum class GeometryLayout : uint8_t
    {
        Triangle,
        TriangleStrip,
        Line,
        Point
    };
}
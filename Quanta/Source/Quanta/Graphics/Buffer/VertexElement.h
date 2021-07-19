#pragma once

#include <stdint.h>

#include "BufferPrimitive.h"

namespace Quanta
{
    struct VertexElement
    {
        BufferPrimitive Primitve = BufferPrimitive::Float;
        uint32_t Count = 0;
        uint32_t Size = 0;
        bool Normalized = false;
    };
}
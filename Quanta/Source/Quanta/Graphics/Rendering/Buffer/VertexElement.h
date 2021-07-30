#pragma once

#include <stddef.h>

#include "BufferPrimitive.h"

namespace Quanta
{
    struct VertexElement final
    {
    public:
        BufferPrimitive Primitve = BufferPrimitive::Float;
        uint32_t Count = 0;
        size_t Size = 0;
        bool Normalized = false;
        
        VertexElement(BufferPrimitive primitive, uint32_t count, size_t size, bool normalized);
    };
}
#pragma once

#include "BufferPrimitive.h"

namespace Quanta
{
    struct VertexElement final
    {
    public:
        BufferPrimitive primitve = BufferPrimitive::Float;
        UInt32 count = 0;
        Size size = 0;
        bool normalized = false;
        
        VertexElement(BufferPrimitive primitive, UInt32 count, Size size, bool normalized);
    };
}
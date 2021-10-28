#pragma once

#include "BufferPrimitive.h"

namespace Quanta
{
    struct VertexElement
    {
        BufferPrimitive primitve = BufferPrimitive::Float;
        U32 count = 0;
        USize size = 0;
        bool normalized = false;
        
        VertexElement(BufferPrimitive primitive, U32 count, USize size, bool normalized);
    };
}
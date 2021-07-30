#pragma once

#include <cstdint>

namespace Quanta
{
    enum class BufferPrimitive : uint8_t
    {
        Float,
        Double, 
        UInt8,
        Int8,
        Int16,
        Int32,
        UInt16,
        UInt32
    };
}
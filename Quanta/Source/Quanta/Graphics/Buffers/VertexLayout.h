#pragma once

#include <vector>
#include <stdint.h>

namespace Quanta
{
    enum class BufferPrimitive
    {
        Float,
        Double, 
        UnsignedByte,
        Char,
        Int16,
        Int32,
        UInt16,
        UInt32
    };
    
    struct VertexElement
    {
        BufferPrimitive Primitve = BufferPrimitive::Float;
        uint32_t Count = 0;
        uint32_t Size = 0;
        bool Normalized = false;
    };
    
    struct VertexLayout final
    {
    public:
        const VertexElement& operator[](uint32_t index) const;

        void Add(const VertexElement& element);

        uint32_t GetCount() const;
        uint32_t GetStride() const;
    private:
        std::vector<VertexElement> elements;
        uint32_t stride = 0;
    };
}
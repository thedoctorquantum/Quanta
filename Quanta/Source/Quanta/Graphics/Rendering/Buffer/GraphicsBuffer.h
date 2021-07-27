#pragma once

#include <memory>
#include <stdint.h>
#include <stddef.h>

#include "BufferUsage.h"

namespace Quanta
{
    class GraphicsBuffer 
    {
    public:
        static std::shared_ptr<GraphicsBuffer> Create(BufferUsage usage, size_t size);
        
        static void Resize(GraphicsBuffer& buffer, size_t size);

        virtual void* MapData() = 0;
        virtual void UnmapData() = 0;
        
        virtual void SetData(const void* data, size_t size) = 0;
        virtual void SetData(const void* data, size_t size, size_t offset) = 0;
        
        virtual size_t GetSize() const = 0;
        virtual BufferUsage GetUsage() const = 0;
    };
}
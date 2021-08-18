#pragma once

#include <memory>

#include "BufferUsage.h"

namespace Quanta
{
    class GraphicsBuffer 
    {
    public:
        static std::shared_ptr<GraphicsBuffer> Create(BufferUsage usage, Size size);
        
        virtual ~GraphicsBuffer() = default;

        virtual void* MapData() = 0;
        virtual void UnmapData() = 0;
        
        virtual void SetData(const void* data, Size size) = 0;
        virtual void SetData(const void* data, Size size, Size offset) = 0;
        
        virtual void Resize(Size size) = 0;     

        virtual Size GetSize() const = 0;
        virtual BufferUsage GetUsage() const = 0;
    };
}
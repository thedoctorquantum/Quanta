#pragma once

#include <memory>

#include "BufferUsage.h"

namespace Quanta
{
    struct GraphicsBuffer 
    {
        static std::shared_ptr<GraphicsBuffer> Create(BufferUsage usage, USize size);
        
        virtual ~GraphicsBuffer() = default;

        virtual void* MapData() = 0;
        virtual void UnmapData() = 0;
        
        virtual void SetData(const void* data, USize size) = 0;
        virtual void SetData(const void* data, USize size, USize offset) = 0;
        
        virtual void Resize(USize size) = 0;     

        virtual USize GetSize() const = 0;
        virtual BufferUsage GetUsage() const = 0;
    };
}
#pragma once

#include <stdint.h>
#include <stddef.h>

#include "BufferUsage.h"

namespace Quanta
{
    class GraphicsBuffer final
    {
    public:
        GraphicsBuffer(BufferUsage usage, size_t size);
        ~GraphicsBuffer();
        
        GraphicsBuffer(const GraphicsBuffer&) = delete; 
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
        
        static void Resize(GraphicsBuffer& buffer, size_t size);

        void* MapData();
        void UnmapData();
        
        void SetData(const void* data, size_t size);
        void SetData(const void* data, size_t size, size_t offset);
        
        uint32_t GetHandle() const;
        size_t GetSize() const;
        BufferUsage GetUsage() const;
    private:
        uint32_t handle;
        size_t size;
        BufferUsage usage;
    };
};
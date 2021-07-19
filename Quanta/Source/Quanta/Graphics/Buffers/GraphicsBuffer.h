#pragma once

#include <stdint.h>

#include "BufferUsage.h"

namespace Quanta
{
    class GraphicsBuffer final
    {
    public:
        GraphicsBuffer(BufferUsage usage, uint32_t size);
        ~GraphicsBuffer();

        static void Resize(GraphicsBuffer& buffer, uint32_t size);

        void* MapData();
        void UnmapData();
        
        void SetData(const void* data, uint32_t size);
        void SetData(const void* data, uint32_t size, uint32_t offset);
        
        uint32_t GetHandle() const;
        uint32_t GetSize() const;
        BufferUsage GetUsage() const;
    private:
        uint32_t handle = 0;
        uint32_t size = 0;
        BufferUsage usage;
    };
};
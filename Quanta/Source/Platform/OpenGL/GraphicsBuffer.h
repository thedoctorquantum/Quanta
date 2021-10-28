#pragma once

#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>

namespace Quanta::OpenGL
{
    struct GraphicsBuffer final : public Quanta::GraphicsBuffer
    {
        GraphicsBuffer(BufferUsage usage, USize size);
        ~GraphicsBuffer();

        GraphicsBuffer(const GraphicsBuffer&) = delete; 
        
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
        
        void* MapData() override;
        void UnmapData() override;
        
        void SetData(const void* data, USize size) override;
        void SetData(const void* data, USize size, USize offset) override;
        
        void Resize(USize size);

        BufferUsage GetUsage() const override;
        USize GetSize() const override;

        U32 GetHandle() const;
    private:
        BufferUsage usage = BufferUsage::Static;
        USize size = 0;
        
        U32 handle = 0;
    };
}
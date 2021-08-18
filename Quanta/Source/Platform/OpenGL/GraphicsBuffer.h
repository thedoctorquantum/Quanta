#pragma once

#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>

namespace Quanta::OpenGL
{
    class GraphicsBuffer final : public Quanta::GraphicsBuffer
    {
    public:
        GraphicsBuffer(BufferUsage usage, Size size);
        ~GraphicsBuffer();

        GraphicsBuffer(const GraphicsBuffer&) = delete; 
        
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
        
        void* MapData() override;
        void UnmapData() override;
        
        void SetData(const void* data, Size size) override;
        void SetData(const void* data, Size size, Size offset) override;
        
        void Resize(Size size);

        BufferUsage GetUsage() const override;
        Size GetSize() const override;

        UInt32 GetHandle() const;
    private:
        BufferUsage usage = BufferUsage::Static;
        Size size = 0;
        
        UInt32 handle = 0;
    };
}
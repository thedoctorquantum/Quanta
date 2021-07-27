#pragma once

#include "../../../Quanta/Graphics/Rendering/Buffer/GraphicsBuffer.h"

namespace Quanta
{
    class OpenGLGraphicsBuffer final : public GraphicsBuffer
    {
    public:
        OpenGLGraphicsBuffer(BufferUsage usage, size_t size);
        ~OpenGLGraphicsBuffer();

        OpenGLGraphicsBuffer(const OpenGLGraphicsBuffer&) = delete; 
        
        OpenGLGraphicsBuffer& operator=(const OpenGLGraphicsBuffer&) = delete;

        static void Resize(OpenGLGraphicsBuffer& buffer, size_t size);

        void* MapData() override;
        void UnmapData() override;
        
        void SetData(const void* data, size_t size) override;
        void SetData(const void* data, size_t size, size_t offset) override;
        
        size_t GetSize() const override;
        BufferUsage GetUsage() const override;

        uint32_t GetHandle() const;
    private:
        size_t size = 0;
        BufferUsage usage = BufferUsage::Static;
        
        uint32_t handle = 0;
    };
}
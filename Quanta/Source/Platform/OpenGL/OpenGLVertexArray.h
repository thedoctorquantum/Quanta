#pragma once

#include <Quanta/Graphics/Buffer/VertexArray.h>
#include <glad/glad.h>

namespace Quanta
{
    class OpenGLVertexArray final : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        OpenGLVertexArray(const OpenGLVertexArray&) = delete;

        OpenGLVertexArray& operator=(OpenGLVertexArray&) = delete;

        void SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout) override;
        void SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type) override;
        
        const std::shared_ptr<GraphicsBuffer>& GetVertexBuffer() const override;
        const std::shared_ptr<GraphicsBuffer>& GetIndexBuffer() const override;
        
        IndexType GetIndexType() const override;
        
        uint32_t GetHandle() const;
        GLenum GetOpenGLIndexType() const;
    private:
        std::shared_ptr<GraphicsBuffer> vertexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> indexBuffer = nullptr;
        
        IndexType indexType = IndexType::UInt32;
        
        uint32_t handle = 0;
        GLenum openglIndexType = 0;
    };
}
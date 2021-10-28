#pragma once

#include <Quanta/Graphics/Buffer/VertexArray.h>

namespace Quanta::OpenGL
{
    struct VertexArray : public Quanta::VertexArray
    {
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;

        VertexArray& operator=(VertexArray&) = delete;

        void SetVertexBuffer(const std::shared_ptr<Quanta::GraphicsBuffer>& buffer, const VertexLayout& layout) override;
        void SetIndexBuffer(const std::shared_ptr<Quanta::GraphicsBuffer>& buffer, IndexType type) override;
        
        const std::shared_ptr<Quanta::GraphicsBuffer>& GetVertexBuffer() const override;
        const std::shared_ptr<Quanta::GraphicsBuffer>& GetIndexBuffer() const override;
        
        IndexType GetIndexType() const override;
                
        U32 GetHandle() const;
    private:
        std::shared_ptr<Quanta::GraphicsBuffer> vertexBuffer = nullptr;
        std::shared_ptr<Quanta::GraphicsBuffer> indexBuffer = nullptr;
        
        IndexType indexType = IndexType::UInt32;
        
        U32 handle = 0;
    };
}
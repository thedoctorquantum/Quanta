#pragma once

#include <memory>

#include "VertexLayout.h"
#include "GraphicsBuffer.h"
#include "IndexType.h"

namespace Quanta
{
    class VertexArray final 
    {
    public:
        VertexArray();
        ~VertexArray();

        void SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout);
        void SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type);
        
        const std::shared_ptr<GraphicsBuffer>& GetVertexBuffer() const;
        const std::shared_ptr<GraphicsBuffer>& GetIndexBuffer() const;
        
        uint32_t GetHandle() const;
        IndexType GetIndexType() const;
    private:
        std::shared_ptr<GraphicsBuffer> vertexBuffer = nullptr;
        std::shared_ptr<GraphicsBuffer> indexBuffer = nullptr;
        
        uint32_t handle;
        IndexType indexType = IndexType::UInt32;
    };
}
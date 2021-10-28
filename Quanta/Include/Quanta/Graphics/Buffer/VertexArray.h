#pragma once

#include <memory>

#include "VertexLayout.h"
#include "GraphicsBuffer.h"
#include "IndexType.h"

namespace Quanta
{
    struct VertexArray 
    {
        static std::shared_ptr<VertexArray> Create();

        virtual ~VertexArray() = default;

        virtual void SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type) = 0;
        
        virtual const std::shared_ptr<GraphicsBuffer>& GetVertexBuffer() const = 0;
        virtual const std::shared_ptr<GraphicsBuffer>& GetIndexBuffer() const = 0;
        
        virtual IndexType GetIndexType() const = 0;
    };
}
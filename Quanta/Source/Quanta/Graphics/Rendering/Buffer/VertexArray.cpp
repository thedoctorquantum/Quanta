#include "VertexArray.h"

#include <glad/glad.h>

#include <iostream>

namespace Quanta
{
    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &handle);
    }
    
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &handle);
    }
    
    void VertexArray::SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout)
    {
        vertexBuffer = buffer;

        glVertexArrayVertexBuffer(handle, 0, buffer->GetHandle(), 0, layout.GetStride());

        uint32_t offset = 0;

        for(int i = 0; i < layout.GetCount(); i++)
        {
            const VertexElement& element = layout[i];

            GLenum type = GL_FLOAT;

            switch(element.Primitve)
            {
            case BufferPrimitive::UInt8:
                type = GL_UNSIGNED_BYTE;
                
                break;
            case BufferPrimitive::Int8:
                type = GL_BYTE;

                break;
            case BufferPrimitive::Float:
                type = GL_FLOAT;

                break;
            case BufferPrimitive::Double:
                type = GL_DOUBLE;

                break;
            }
            
            glEnableVertexArrayAttrib(handle, i);
            
            glVertexArrayAttribFormat(handle, i, element.Count, type, element.Normalized, offset);

            glVertexArrayAttribBinding(handle, i, 0);

            offset += element.Count * element.Size;
        }
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type)
    {
        indexBuffer = buffer;
        indexType = type;

        glVertexArrayElementBuffer(handle, buffer->GetHandle());
    }
    
    const std::shared_ptr<GraphicsBuffer>& VertexArray::GetVertexBuffer() const
    {
        return vertexBuffer;
    }

    const std::shared_ptr<GraphicsBuffer>& VertexArray::GetIndexBuffer() const
    {
        return indexBuffer;
    }

    uint32_t VertexArray::GetHandle() const
    {
        return handle;
    }

    IndexType VertexArray::GetIndexType() const
    {
        return indexType;
    }
}
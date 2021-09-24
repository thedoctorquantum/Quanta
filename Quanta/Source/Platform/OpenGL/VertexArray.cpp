#include <glad/glad.h>

#include "VertexArray.h"
#include "GraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &handle);

        DEBUG_ASSERT(handle != 0);
    }
    
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &handle);
    }
    
    void VertexArray::SetVertexBuffer(const std::shared_ptr<Quanta::GraphicsBuffer>& buffer, const VertexLayout& layout)
    {
        DEBUG_ASSERT(buffer != nullptr);
        
        vertexBuffer = buffer;

        DEBUG_ASSERT(layout.GetCount() != 0);
        DEBUG_ASSERT(layout.GetStride() != 0);

        const GraphicsBuffer* glBuffer = nullptr;

        if constexpr (DEBUG)
        {
            glBuffer = dynamic_cast<const GraphicsBuffer*>(buffer.get());

            DEBUG_ASSERT(glBuffer != nullptr);
        }
        else
        {
            glBuffer = reinterpret_cast<const GraphicsBuffer*>(buffer.get());
        }
        
        glVertexArrayVertexBuffer(handle, 0, glBuffer->GetHandle(), 0, layout.GetStride());

        USize offset = 0;

        for(USize i = 0; i < layout.GetCount(); i++)
        {
            const auto& element = layout[i];

            DEBUG_ASSERT(
                element.primitve == BufferPrimitive::UInt8 ||
                element.primitve == BufferPrimitive::Int8 ||
                element.primitve == BufferPrimitive::Float ||
                element.primitve == BufferPrimitive::Double
            );

            GLenum type = 0;

            switch(element.primitve)
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
            
            glVertexArrayAttribFormat(handle, i, element.count, type, element.normalized, offset);

            glVertexArrayAttribBinding(handle, i, 0);

            offset += element.count * element.size;
        }
    }
    
    void VertexArray::SetIndexBuffer(const std::shared_ptr<Quanta::GraphicsBuffer>& buffer, const IndexType type)
    {
        DEBUG_ASSERT(buffer != nullptr);

        indexBuffer = buffer;
        indexType = type;

        const GraphicsBuffer* glBuffer = nullptr;

        if constexpr (DEBUG)
        {
            glBuffer = dynamic_cast<const GraphicsBuffer*>(buffer.get());

            DEBUG_ASSERT(glBuffer != nullptr);
        }
        else
        {
            glBuffer = reinterpret_cast<const GraphicsBuffer*>(buffer.get());
        }

        glVertexArrayElementBuffer(handle, glBuffer->GetHandle());
    }
    
    const std::shared_ptr<Quanta::GraphicsBuffer>& VertexArray::GetVertexBuffer() const
    {
        return vertexBuffer;
    }

    const std::shared_ptr<Quanta::GraphicsBuffer>& VertexArray::GetIndexBuffer() const
    {
        return indexBuffer;
    }

    IndexType VertexArray::GetIndexType() const
    {
        return indexType;
    }
    
    U32 VertexArray::GetHandle() const
    {
        return handle;
    }
}
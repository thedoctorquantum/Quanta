#include <glad/glad.h>

#include "OpenGLVertexArray.h"
#include "OpenGLGraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &handle);

        DEBUG_ASSERT(handle != 0);
    }
    
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &handle);
    }
    
    void OpenGLVertexArray::SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout)
    {
        DEBUG_ASSERT(buffer != nullptr);
        DEBUG_ASSERT(layout.GetCount() != 0);
        DEBUG_ASSERT(layout.GetStride() != 0);

        vertexBuffer = buffer;

        OpenGLGraphicsBuffer* glBuffer = (OpenGLGraphicsBuffer*) buffer.get();

        DEBUG_ASSERT(glBuffer != nullptr);

        glVertexArrayVertexBuffer(handle, 0, glBuffer->GetHandle(), 0, layout.GetStride());

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
            default:
                DEBUG_FAILURE_MESSAGE("element.Primitive was out of range");
            }
            
            glEnableVertexArrayAttrib(handle, i);
            
            glVertexArrayAttribFormat(handle, i, element.Count, type, element.Normalized, offset);

            glVertexArrayAttribBinding(handle, i, 0);

            offset += element.Count * element.Size;
        }
    }
    
    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type)
    {
        DEBUG_ASSERT(buffer != nullptr);

        OpenGLGraphicsBuffer* glBuffer = (OpenGLGraphicsBuffer*) buffer.get();

        DEBUG_ASSERT(glBuffer != nullptr);

        indexBuffer = buffer;

        glVertexArrayElementBuffer(handle, glBuffer->GetHandle());

        switch(type)
        {
        case IndexType::UInt8:
            openglIndexType = GL_UNSIGNED_BYTE;

            break;  
        case IndexType::UInt16: 
            openglIndexType = GL_UNSIGNED_SHORT;

            break;
        case IndexType::UInt32:
            openglIndexType = GL_UNSIGNED_INT;

            break;
        default: 
            DEBUG_FAILURE_MESSAGE("type was out of range");
        }
        
        indexType = type;
    }
    
    const std::shared_ptr<GraphicsBuffer>& OpenGLVertexArray::GetVertexBuffer() const
    {
        return vertexBuffer;
    }

    const std::shared_ptr<GraphicsBuffer>& OpenGLVertexArray::GetIndexBuffer() const
    {
        return indexBuffer;
    }

    IndexType OpenGLVertexArray::GetIndexType() const
    {
        return indexType;
    }

    uint32_t OpenGLVertexArray::GetHandle() const
    {
        return handle;
    }
    
    GLenum OpenGLVertexArray::GetOpenGLIndexType() const
    {
        return openglIndexType;
    }
}
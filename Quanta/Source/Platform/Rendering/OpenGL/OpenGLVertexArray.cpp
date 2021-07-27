#include <glad/glad.h>

#include "OpenGLVertexArray.h"
#include "OpenGLGraphicsBuffer.h"

namespace Quanta
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &handle);
    }
    
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &handle);
    }
    
    void OpenGLVertexArray::SetVertexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, const VertexLayout& layout)
    {
        vertexBuffer = buffer;

        OpenGLGraphicsBuffer& glBuffer = (OpenGLGraphicsBuffer&) *buffer;

        glVertexArrayVertexBuffer(handle, 0, glBuffer.GetHandle(), 0, layout.GetStride());

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
    
    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<GraphicsBuffer>& buffer, IndexType type)
    {
        OpenGLGraphicsBuffer& glBuffer = (OpenGLGraphicsBuffer&) *buffer;

        indexBuffer = buffer;
        indexType = type;

        glVertexArrayElementBuffer(handle, glBuffer.GetHandle());

        switch(indexType)
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
        }
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
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
        
        vertexBuffer = buffer;

        DEBUG_ASSERT(layout.GetCount() != 0);
        DEBUG_ASSERT(layout.GetStride() != 0);

        const OpenGLGraphicsBuffer* glBuffer = nullptr;

#if DEBUG
        glBuffer = dynamic_cast<const OpenGLGraphicsBuffer*>(buffer.get());

        DEBUG_ASSERT(glBuffer != nullptr);
#else
        glBuffer = reinterpret_cast<const OpenGLGraphicsBuffer*>(buffer.get());
#endif
        
        glVertexArrayVertexBuffer(handle, 0, glBuffer->GetHandle(), 0, layout.GetStride());

        size_t offset = 0;

        for(int i = 0; i < layout.GetCount(); i++)
        {
            const VertexElement& element = layout[i];

            GLenum type = 0;

            DEBUG_ASSERT(
                element.Primitve == BufferPrimitive::UInt8 ||
                element.Primitve == BufferPrimitive::Int8 ||
                element.Primitve == BufferPrimitive::Float ||
                element.Primitve == BufferPrimitive::Double
            );

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
        indexType = type;

        DEBUG_ASSERT(buffer != nullptr);

        indexBuffer = buffer;

        const OpenGLGraphicsBuffer* glBuffer = nullptr;

#if DEBUG
        glBuffer = dynamic_cast<const OpenGLGraphicsBuffer*>(buffer.get());

        DEBUG_ASSERT(glBuffer != nullptr);
#else
        glBuffer = reinterpret_cast<const OpenGLGraphicsBuffer*>(buffer.get());
#endif

        glVertexArrayElementBuffer(handle, glBuffer->GetHandle());
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
}
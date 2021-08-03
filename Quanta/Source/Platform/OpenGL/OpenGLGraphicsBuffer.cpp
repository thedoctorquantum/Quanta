#include <glad/glad.h>

#include "OpenGLGraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(BufferUsage usage, size_t size)
    {       
        this->usage = usage;
        this->size = size;
        
        glCreateBuffers(1, &handle);
        
        DEBUG_ASSERT(handle != 0);

        switch(usage)
        {
        case BufferUsage::Static:
            glNamedBufferStorage(handle, size, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

            break;
        case BufferUsage::Dynamic:
            glNamedBufferData(handle, size, nullptr, GL_DYNAMIC_DRAW);

            break;
        default:
            DEBUG_FAILURE_MESSAGE("GraphicsBuffer usage is not supported!");
        }
    }
    
    OpenGLGraphicsBuffer::~OpenGLGraphicsBuffer()
    {
        glDeleteBuffers(1, &handle);
    }
    
    void OpenGLGraphicsBuffer::Resize(OpenGLGraphicsBuffer& buffer, size_t size)
    {
        DEBUG_ASSERT(buffer.GetUsage() != BufferUsage::Static);

        glNamedBufferData(buffer.handle, size, nullptr, GL_DYNAMIC_DRAW);

        buffer.size = size;
    }

    void* OpenGLGraphicsBuffer::MapData()
    {
        return glMapNamedBuffer(handle, GL_READ_WRITE);
    }

    void OpenGLGraphicsBuffer::UnmapData()
    {
        glUnmapNamedBuffer(handle);
    }
    
    void OpenGLGraphicsBuffer::SetData(const void* data, size_t size)
    {
        glNamedBufferSubData(handle, 0, size, data);
    }

    void OpenGLGraphicsBuffer::SetData(const void* data, size_t size, size_t offset)
    {
        glNamedBufferSubData(handle, offset, size, data);
    }

    uint32_t OpenGLGraphicsBuffer::GetHandle() const
    {
        return handle;
    }

    size_t OpenGLGraphicsBuffer::GetSize() const
    {
        return size;
    }

    BufferUsage OpenGLGraphicsBuffer::GetUsage() const
    {
        return usage;
    }
}
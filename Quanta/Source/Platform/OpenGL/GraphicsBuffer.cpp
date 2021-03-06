#include <glad/glad.h>

#include "GraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGL::GraphicsBuffer::GraphicsBuffer(const BufferUsage usage, const USize size)
    {       
        DEBUG_ASSERT(
            usage == BufferUsage::Static ||
            usage == BufferUsage::Dynamic
        );

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
        }
    }
    
    OpenGL::GraphicsBuffer::~GraphicsBuffer()
    {
        glDeleteBuffers(1, &handle);
    }
    
    void* OpenGL::GraphicsBuffer::MapData()
    {
        return glMapNamedBuffer(handle, GL_READ_WRITE);
    }

    void OpenGL::GraphicsBuffer::UnmapData()
    {
        glUnmapNamedBuffer(handle);
    }
    
    void OpenGL::GraphicsBuffer::SetData(const void* const data, const USize size)
    {
        glNamedBufferSubData(handle, 0, size, data);
    }

    void OpenGL::GraphicsBuffer::SetData(const void* data, const USize size, const USize offset)
    {
        glNamedBufferSubData(handle, offset, size, data);
    }

    void OpenGL::GraphicsBuffer::Resize(const USize size)
    {
        DEBUG_ASSERT(usage != BufferUsage::Static);

        glNamedBufferData(handle, size, nullptr, GL_DYNAMIC_DRAW);

        this->size = size;
    }

    BufferUsage OpenGL::GraphicsBuffer::GetUsage() const
    {
        return usage;
    }

    USize OpenGL::GraphicsBuffer::GetSize() const
    {
        return size;
    }

    U32 OpenGL::GraphicsBuffer::GetHandle() const
    {
        return handle;
    }
}
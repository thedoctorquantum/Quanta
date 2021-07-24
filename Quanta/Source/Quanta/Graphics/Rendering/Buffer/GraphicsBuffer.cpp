#include <glad/glad.h>
#include <assert.h>

#include "GraphicsBuffer.h"

namespace Quanta
{
    GraphicsBuffer::GraphicsBuffer(BufferUsage usage, size_t size)
    {        
        this->usage = usage;
        this->size = size;
        
        glCreateBuffers(1, &handle);
        
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
    
    GraphicsBuffer::~GraphicsBuffer()
    {
        glDeleteBuffers(1, &handle);
    }

    void GraphicsBuffer::Resize(GraphicsBuffer& buffer, size_t size)
    {
        assert(buffer.usage != BufferUsage::Static);

        glNamedBufferData(buffer.handle, size, nullptr, GL_DYNAMIC_DRAW);

        buffer.size = size;
    }

    void* GraphicsBuffer::MapData()
    {
        return glMapNamedBuffer(handle, GL_READ_WRITE);
    }

    void GraphicsBuffer::UnmapData()
    {
        glUnmapNamedBuffer(handle);
    }
    
    void GraphicsBuffer::SetData(const void* data, size_t size)
    {
        glNamedBufferSubData(handle, 0, size, data);
    }

    void GraphicsBuffer::SetData(const void* data, size_t size, size_t offset)
    {
        glNamedBufferSubData(handle, offset, size, data);
    }

    uint32_t GraphicsBuffer::GetHandle() const
    {
        return handle;
    }

    size_t GraphicsBuffer::GetSize() const
    {
        return size;
    }

    BufferUsage GraphicsBuffer::GetUsage() const
    {
        return usage;
    }
}
#include "GraphicsBuffer.h"
#include "glad/glad.h"

#include <assert.h>

namespace Quanta
{
    GraphicsBuffer::GraphicsBuffer(BufferUsage usage, uint32_t size)
    {        
        glCreateBuffers(1, &handle);

        switch (usage)
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

    void GraphicsBuffer::Resize(GraphicsBuffer& buffer, uint32_t size)
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

    void GraphicsBuffer::SetData(void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(handle, offset, size, data);
    }

    uint32_t GraphicsBuffer::GetHandle() const
    {
        return handle;
    }

    uint32_t GraphicsBuffer::GetSize() const
    {
        return size;
    }

    BufferUsage GraphicsBuffer::GetUsage() const
    {
        return usage;
    }
}
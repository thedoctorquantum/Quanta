#include <glad/glad.h>

#include "OpenGLTexture2DArray.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLTexture2DArray::OpenGLTexture2DArray(size_t width, size_t height, size_t depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &handle);
        
        DEBUG_ASSERT(handle != 0);

        glTextureStorage3D(handle, 1, GL_RGBA8, width, height, depth);
    }

    OpenGLTexture2DArray::~OpenGLTexture2DArray()
    {
        glDeleteTextures(1, &handle);
    }

    void OpenGLTexture2DArray::SetData(const void* data, size_t index)
    {
        glTextureSubImage3D(handle, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    
    void OpenGLTexture2DArray::GetData(void* data, size_t index) const
    {
        DEBUG_FAILURE_MESSAGE("GetData is not currently implemented");
    }
    
    size_t OpenGLTexture2DArray::GetWidth() const
    {
        return width;
    }
    
    size_t OpenGLTexture2DArray::GetHeight() const
    {
        return height;
    }

    size_t OpenGLTexture2DArray::GetDepth() const 
    {
        return depth;
    }

    uint32_t OpenGLTexture2DArray::GetHandle() const
    {
        return handle;
    }
}
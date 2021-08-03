#include <glad/glad.h>

#include "OpenGLCubeMap.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLCubeMap::OpenGLCubeMap(size_t width, size_t height)
    {
        this->width = width;
        this->height = height;
        
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &handle);

        DEBUG_ASSERT(handle != 0);
        
        glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
    }
    
    OpenGLCubeMap::~OpenGLCubeMap()
    {
        glDeleteTextures(1, &handle);
    }

    void OpenGLCubeMap::SetData(const void* data, size_t index)
    {
        glTextureSubImage3D(handle, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLCubeMap::GetData(void* data, size_t index) const
    {
        glGetTextureSubImage(handle, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, width * height, data);
    }    
    
    size_t OpenGLCubeMap::GetWidth() const
    {
        return width;
    }

    size_t OpenGLCubeMap::GetHeight() const
    {
        return height;
    }

    uint32_t OpenGLCubeMap::GetHandle() const
    {
        return handle;
    }
}
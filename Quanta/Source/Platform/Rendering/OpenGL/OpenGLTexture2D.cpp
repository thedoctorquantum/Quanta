#include <glad/glad.h>

#include "OpenGLTexture2D.h"

namespace Quanta
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &handle);
        
        this->width = width;
        this->height = height;

        glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
    }
    
    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &handle);
    }
    
    void OpenGLTexture2D::SetData(const void* data)
    {
        glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::GetData(void* data) const
    {
        glGetTextureSubImage(handle, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, width * height, data);
    }

    uint32_t OpenGLTexture2D::GetWidth() const
    {
        return width;
    }

    uint32_t OpenGLTexture2D::GetHeight() const
    {
        return height;
    }

    uint32_t OpenGLTexture2D::GetHandle() const
    {
        return handle;
    }
};
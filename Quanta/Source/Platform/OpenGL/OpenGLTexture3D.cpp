#include <glad/glad.h>

#include "OpenGLTexture3D.h"

namespace Quanta
{
    OpenGLTexture3D::OpenGLTexture3D(size_t width, size_t height, size_t depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;

        glCreateTextures(GL_TEXTURE_3D, 1, &handle);

        glTextureStorage3D(handle, 1, GL_RGBA8, width, height, depth);
    }

    OpenGLTexture3D::~OpenGLTexture3D()
    {
        glDeleteTextures(1, &handle);
    }
    
    void OpenGLTexture3D::SetData(const void* data)
    {
        glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } 

    void OpenGLTexture3D::GetData(void* data) const
    {
        glGetTextureSubImage(handle, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, width * height * depth, data);
    }

    size_t OpenGLTexture3D::GetWidth() const
    {
        return width;
    }

    size_t OpenGLTexture3D::GetHeight() const
    {
        return height;
    }

    size_t OpenGLTexture3D::GetDepth() const
    {
        return depth;
    }

    uint32_t OpenGLTexture3D::GetHandle() const
    {
        return handle;
    }
}
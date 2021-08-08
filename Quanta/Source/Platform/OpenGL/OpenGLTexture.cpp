#include <glad/glad.h>

#include "OpenGLTexture.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLTexture::OpenGLTexture(TextureType type, size_t width, size_t height, size_t depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;

        DEBUG_ASSERT(
            type == TextureType::Texture1D ||
            type == TextureType::Texture2D ||
            type == TextureType::Texture3D ||
            type == TextureType::CubeMap 
        );

        this->type = type;

        GLenum textureTarget = 0;

        switch(type)
        {
        case TextureType::Texture1D:
            textureTarget = GL_TEXTURE_1D;

            break;
        case TextureType::Texture2D:
            textureTarget = GL_TEXTURE_2D;

            break;
        case TextureType::Texture3D:
            textureTarget = GL_TEXTURE_3D;

            break;
        case TextureType::CubeMap:
            textureTarget = GL_TEXTURE_CUBE_MAP;

            break;
        } 

        glCreateTextures(textureTarget, 1, &handle);

        DEBUG_ASSERT(handle != 0);

        switch(type)
        {
        case TextureType::Texture1D:
            glTextureStorage1D(handle, 1, GL_RGBA8, width);

            break;
        case TextureType::Texture2D:
            glTextureStorage2D(handle, 1, GL_RGBA8, width, height);

            break;
        case TextureType::Texture3D:
            glTextureStorage3D(handle, 1, GL_RGBA8, width, height, depth);

            break;
        case TextureType::CubeMap:
            glTextureStorage2D(handle, 1, GL_RGBA8, width, height);

            break;
        } 
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &handle);
    }
    
    void OpenGLTexture::SetData(const void* data)
    {
        DEBUG_ASSERT(data != nullptr);

        switch(type)
        {
        case TextureType::Texture1D:
            glTextureSubImage1D(handle, 0, 0, width, GL_RGBA, GL_UNSIGNED_BYTE, data);

            break;
        case TextureType::Texture2D:
            glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

            break;
        case TextureType::Texture3D:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data);

            break;
        case TextureType::CubeMap:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, 6, GL_RGBA, GL_UNSIGNED_BYTE, data);

            break;
        } 
    }

    TextureType OpenGLTexture::GetType() const
    {
        return type;
    }

    size_t OpenGLTexture::GetWidth() const
    {
        return width;
    }

    size_t OpenGLTexture::GetHeight() const
    {
        return height;
    }

    size_t OpenGLTexture::GetDepth() const
    {
        return depth;
    }
    
    uint32_t OpenGLTexture::GetHandle() const
    {
        return handle;
    }
}
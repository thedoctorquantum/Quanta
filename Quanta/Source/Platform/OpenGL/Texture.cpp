#include <glad/glad.h>

#include "Texture.h"
#include "Conversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    Texture::Texture(TextureType type, TexelFormat format, USize width, USize height, USize depth)
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

        glPixelFormat = TexelFormatToPixelFormat(format);
        glInternalFormat = TexelFormatToInternalFormat(format);
        glPixelType = TexelFormatToPixelType(format);

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
            glTextureStorage1D(handle, 1, glInternalFormat, width);

            break;
        case TextureType::Texture2D:
            glTextureStorage2D(handle, 1, glInternalFormat, width, height);

            break;
        case TextureType::Texture3D:
            glTextureStorage3D(handle, 1, glInternalFormat, width, height, depth);

            break;
        case TextureType::CubeMap:
            glTextureStorage2D(handle, 1, glInternalFormat, width, height);

            break;
        } 
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &handle);
    }
    
    void Texture::SetData(const void* data)
    {
        DEBUG_ASSERT(data != nullptr);

        switch(type)
        {
        case TextureType::Texture1D:
            glTextureSubImage1D(handle, 0, 0, width, glPixelFormat, glPixelType, data);

            break;
        case TextureType::Texture2D:
            glTextureSubImage2D(handle, 0, 0, 0, width, height, glPixelFormat, glPixelType, data);

            break;
        case TextureType::Texture3D:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, depth, glPixelFormat, glPixelType, data);

            break;
        case TextureType::CubeMap:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, 6, glPixelFormat, glPixelType, data);

            break;
        } 
    }
    
    void Texture::SetData(const void* data, USize xOffset, USize yOffset, USize zOffset)
    {
        DEBUG_ASSERT(data != nullptr);

        switch(type)
        {
        case TextureType::Texture1D:
            glTextureSubImage1D(handle, 0, xOffset, width, glPixelFormat, glPixelType, data);

            break;
        case TextureType::Texture2D:
            glTextureSubImage2D(handle, 0, xOffset, yOffset, width, height, glPixelFormat, glPixelType, data);

            break;
        case TextureType::Texture3D:
            glTextureSubImage3D(handle, 0, xOffset, yOffset, zOffset, width, height, depth, glPixelFormat, glPixelType, data);

            break;
        case TextureType::CubeMap:
            glTextureSubImage3D(handle, 0, xOffset, yOffset, zOffset, width, height, 1, glPixelFormat, glPixelType, data);

            break;
        } 
    }

    TextureType Texture::GetType() const
    {
        return type;
    }

    TexelFormat Texture::GetFormat() const
    {
        return format;
    }

    USize Texture::GetWidth() const
    {
        return width;
    }

    USize Texture::GetHeight() const
    {
        return height;
    }

    USize Texture::GetDepth() const
    {
        return depth;
    }
    
    U32 Texture::GetHandle() const
    {
        return handle;
    }
}
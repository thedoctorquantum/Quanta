#include <glad/glad.h>

#include "Texture.h"
#include "Conversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    Texture::Texture(const Type type, const TexelFormat format, const USize width, const USize height, const USize depth)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;

        DEBUG_ASSERT(
            type == Type::Texture1D ||
            type == Type::Texture2D ||
            type == Type::Texture3D ||
            type == Type::CubeMap 
        );
        
        this->type = type;

        glPixelFormat = TexelFormatToPixelFormat(format);
        glInternalFormat = TexelFormatToInternalFormat(format);
        glPixelType = TexelFormatToPixelType(format);

        GLenum textureTarget = 0;

        switch(type)
        {
        case Type::Texture1D:
            textureTarget = GL_TEXTURE_1D;

            break;
        case Type::Texture2D:
            textureTarget = GL_TEXTURE_2D;

            break;
        case Type::Texture3D:
            textureTarget = GL_TEXTURE_3D;

            break;
        case Type::CubeMap:
            textureTarget = GL_TEXTURE_CUBE_MAP;

            break;
        } 

        glCreateTextures(textureTarget, 1, &handle);

        DEBUG_ASSERT(handle != 0);

        switch(type)
        {
        case Type::Texture1D:
            glTextureStorage1D(handle, 1, glInternalFormat, width);

            break;
        case Type::Texture2D:
            glTextureStorage2D(handle, 1, glInternalFormat, width, height);

            break;
        case Type::Texture3D:
            glTextureStorage3D(handle, 1, glInternalFormat, width, height, depth);

            break;
        case Type::CubeMap:
            glTextureStorage2D(handle, 1, glInternalFormat, width, height);

            break;
        } 
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &handle);
    }
    
    void Texture::SetData(const void* const data)
    {
        DEBUG_ASSERT(data != nullptr);

        switch(type)
        {
        case Type::Texture1D:
            glTextureSubImage1D(handle, 0, 0, width, glPixelFormat, glPixelType, data);

            break;
        case Type::Texture2D:
            glTextureSubImage2D(handle, 0, 0, 0, width, height, glPixelFormat, glPixelType, data);

            break;
        case Type::Texture3D:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, depth, glPixelFormat, glPixelType, data);

            break;
        case Type::CubeMap:
            glTextureSubImage3D(handle, 0, 0, 0, 0, width, height, 6, glPixelFormat, glPixelType, data);

            break;
        } 
    }
    
    void Texture::SetData(const void* const data, const USize xOffset, const USize yOffset, const USize zOffset)
    {
        DEBUG_ASSERT(data != nullptr);

        switch(type)
        {
        case Type::Texture1D:
            glTextureSubImage1D(handle, 0, xOffset, width, glPixelFormat, glPixelType, data);

            break;
        case Type::Texture2D:
            glTextureSubImage2D(handle, 0, xOffset, yOffset, width, height, glPixelFormat, glPixelType, data);

            break;
        case Type::Texture3D:
            glTextureSubImage3D(handle, 0, xOffset, yOffset, zOffset, width, height, depth, glPixelFormat, glPixelType, data);

            break;
        case Type::CubeMap:
            glTextureSubImage3D(handle, 0, xOffset, yOffset, zOffset, width, height, 1, glPixelFormat, glPixelType, data);

            break;
        } 
    }

    void Texture::Clear(const void* const data)
    {
        glClearTexImage(handle, 0, glPixelFormat, glPixelType, data);
    }

    void Texture::GetData(
        void* const data,
        const USize x, 
        const USize y, 
        const USize z, 
        const USize width, 
        const USize height, 
        const USize depth) const
    {
        DEBUG_ASSERT(x + width <= this->width);
        DEBUG_ASSERT(y + height <= this->height);
        DEBUG_ASSERT(z + depth <= this->depth);

        glGetTextureSubImage(handle, 0, x, y, z, width, height, depth, glPixelFormat, glPixelType, width * height * depth, data);
    }

    Texture::Type Texture::GetType() const
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

    GLenum Texture::GetGLPixelFormat() const
    {
        return glPixelFormat;
    }

    GLenum Texture::GetGLInternalFormat() const
    {
        return glInternalFormat;
    }
    
    GLenum Texture::GetGLPixelType() const
    {
        return glPixelType;
    }
}
#pragma once

#include <Quanta/Graphics/Texture/Texture.h>
#include <glad/glad.h>

namespace Quanta::OpenGL
{
    struct Texture : public Quanta::Texture
    {
        Texture(Type type, TexelFormat format, USize width, USize height, USize depth);
        ~Texture();

        void SetData(const void* data) override;
        void SetData(const void* data, USize xOffset, USize yOffset, USize zOffset) override;

        void GetData(void* data, USize x, USize y, USize z, USize width, USize height, USize depth) const override;

        void Clear(const void* data) override;

        Type GetType() const override;
        TexelFormat GetFormat() const override;

        USize GetWidth() const override;
        USize GetHeight() const override;
        USize GetDepth() const override;

        U32 GetHandle() const;

        GLenum GetGLPixelFormat() const;
        GLenum GetGLInternalFormat() const;
        GLenum GetGLPixelType() const;
    private:
        Type type = Type::Texture1D;
        TexelFormat format = TexelFormat::Rgba8I;

        USize width = 0;
        USize height = 0;
        USize depth = 0;

        U32 handle = 0;

        GLenum glPixelFormat = 0;
        GLenum glInternalFormat = 0;
        GLenum glPixelType = 0;
    };
}
#pragma once

#include <Quanta/Graphics/Texture/Texture.h>
#include <glad/glad.h>

namespace Quanta::OpenGL
{
    class Texture final : public Quanta::Texture
    {
    public:
        Texture(TextureType type, TexelFormat format, USize width, USize height, USize depth);
        ~Texture();

        void SetData(const void* data) override;

        void SetData(const void* data, USize xOffset, USize yOffset, USize zOffset) override;

        TextureType GetType() const override;
        TexelFormat GetFormat() const override;

        USize GetWidth() const override;
        USize GetHeight() const override;
        USize GetDepth() const override;

        U32 GetHandle() const;
    private:
        TextureType type = TextureType::Texture1D;
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
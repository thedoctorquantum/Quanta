#pragma once

#include <Quanta/Graphics/Texture/Texture.h>
#include <glad/glad.h>

namespace Quanta::OpenGL
{
    class Texture final : public Quanta::Texture
    {
    public:
        Texture(TextureType type, TexelFormat format, Size width, Size height, Size depth);
        ~Texture();

        void SetData(const void* data) override;

        void SetData(const void* data, Size xOffset, Size yOffset, Size zOffset) override;

        TextureType GetType() const override;
        TexelFormat GetFormat() const override;

        Size GetWidth() const override;
        Size GetHeight() const override;
        Size GetDepth() const override;

        UInt32 GetHandle() const;
    private:
        TextureType type = TextureType::Texture1D;
        TexelFormat format = TexelFormat::Rgba8I;

        Size width = 0;
        Size height = 0;
        Size depth = 0;

        UInt32 handle = 0;

        GLenum glPixelFormat = 0;
        GLenum glInternalFormat = 0;
        GLenum glPixelType = 0;
    };
}
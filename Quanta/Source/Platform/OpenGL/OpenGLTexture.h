#pragma once

#include <Quanta/Graphics/Texture/Texture.h>
#include <glad/glad.h>

namespace Quanta
{
    class OpenGLTexture final : public Texture
    {
    public:
        OpenGLTexture(TextureType type, TexelFormat format, size_t width, size_t height, size_t depth);
        ~OpenGLTexture();

        void SetData(const void* data) override;

        void SetData(const void* data, size_t xOffset, size_t yOffset, size_t zOffset) override;

        TextureType GetType() const override;
        TexelFormat GetFormat() const override;

        size_t GetWidth() const override;
        size_t GetHeight() const override;
        size_t GetDepth() const override;

        uint32_t GetHandle() const;
    private:
        TextureType type = TextureType::Texture1D;
        TexelFormat format = TexelFormat::Rgba8I;

        size_t width = 0;
        size_t height = 0;
        size_t depth = 0;

        uint32_t handle = 0;

        GLenum glPixelFormat = 0;
        GLenum glInternalFormat = 0;
        GLenum glPixelType = 0;
    };
}
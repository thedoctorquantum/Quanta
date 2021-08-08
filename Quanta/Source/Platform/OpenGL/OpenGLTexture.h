#pragma once

#include <Quanta/Graphics/Texture/Texture.h>

namespace Quanta
{
    class OpenGLTexture final : public Texture
    {
    public:
        OpenGLTexture(TextureType type, size_t width, size_t height, size_t depth);
        ~OpenGLTexture();

        void SetData(const void* data) override;

        TextureType GetType() const override;

        size_t GetWidth() const override;
        size_t GetHeight() const override;
        size_t GetDepth() const override;

        uint32_t GetHandle() const;
    private:
        TextureType type = TextureType::Texture1D;

        size_t width = 0;
        size_t height = 0;
        size_t depth = 0;

        uint32_t handle = 0;
    };
}
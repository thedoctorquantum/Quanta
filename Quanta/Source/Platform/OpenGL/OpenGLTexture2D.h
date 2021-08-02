#pragma once

#include <Quanta/Graphics/Texture/Texture2D.h>

namespace Quanta
{
    class OpenGLTexture2D final : public Texture2D
    {
    public:
        OpenGLTexture2D(size_t width, size_t height);
        ~OpenGLTexture2D();

        OpenGLTexture2D(const OpenGLTexture2D&) = delete;        
        OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

        void SetData(const void* data) override;
        void GetData(void* data) const override;

        size_t GetWidth() const override;
        size_t GetHeight() const override;

        uint32_t GetHandle() const; 
    private:
        size_t width = 0;
        size_t height = 0;

        uint32_t handle = 0;
    };
}

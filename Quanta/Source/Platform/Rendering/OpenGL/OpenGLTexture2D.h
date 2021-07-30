#pragma once

#include "../../../Quanta/Graphics/Rendering/Texture/Texture2D.h"

namespace Quanta
{
    class OpenGLTexture2D final : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        ~OpenGLTexture2D();

        OpenGLTexture2D(const OpenGLTexture2D&) = delete;
        
        OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

        void SetData(const void* data) override;
        void GetData(void* data) const override;

        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;

        uint32_t GetHandle() const; 
    private:
        uint32_t width = 0;
        uint32_t height = 0;

        uint32_t handle = 0;
    };
}

#pragma once

#include <Quanta/Graphics/Rendering/Texture/Texture3D.h>

namespace Quanta
{
    class OpenGLTexture3D final : public Texture3D
    {
    public:
        OpenGLTexture3D(size_t width, size_t height, size_t depth);
        ~OpenGLTexture3D();

        void SetData(const void* data) override;
        void GetData(void* data) const override;
        
        size_t GetWidth() const override;
        size_t GetHeight() const override;
        size_t GetDepth() const override;
        
        uint32_t GetHandle() const;
    private:
        uint32_t handle = 0;

        size_t width = 0;
        size_t height = 0;
        size_t depth = 0;
    };
}
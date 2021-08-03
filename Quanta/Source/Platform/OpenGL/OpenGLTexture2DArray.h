#pragma once

#include <Quanta/Graphics/Texture/Texture2DArray.h>

namespace Quanta
{
    class OpenGLTexture2DArray final : public Texture2DArray
    {
    public:
        OpenGLTexture2DArray(size_t width, size_t height, size_t depth);
        ~OpenGLTexture2DArray();

        void SetData(const void* data, size_t index) override;
        void GetData(void* data, size_t index) const override;
        
        size_t GetWidth() const override;
        size_t GetHeight() const override;
        size_t GetDepth() const override;

        uint32_t GetHandle() const;
    private:
        size_t width = 0;
        size_t height = 0;
        size_t depth = 0;

        uint32_t handle = 0;
    };
}
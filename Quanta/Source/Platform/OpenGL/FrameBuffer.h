#pragma once

#include <Quanta/Graphics/Texture/FrameBuffer.h>

namespace Quanta::OpenGL
{
    class FrameBuffer final : public Quanta::FrameBuffer
    {
    public:
        FrameBuffer(const Description& description);
        ~FrameBuffer();

        size_t GetWidth() const override;
        size_t GetHeight() const override;

        void ClearTexture(size_t index, const glm::vec4& color) override;

        std::shared_ptr<Texture> GetTexture(size_t index) const override;
    private:
        size_t width = 0;
        size_t height = 0;

        std::vector<std::shared_ptr<Texture>> textures;

        uint32_t handle = 0;
    };
}
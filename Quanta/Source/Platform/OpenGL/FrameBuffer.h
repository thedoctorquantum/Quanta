#pragma once

#include <Quanta/Graphics/Texture/FrameBuffer.h>

namespace Quanta::OpenGL
{
    struct FrameBuffer : public Quanta::FrameBuffer
    {
        FrameBuffer(const Description& description);
        ~FrameBuffer();

        void Clear(const glm::vec4& color, float depth, std::int32_t stencil) override;

        std::shared_ptr<Quanta::Texture> GetColorTexture(std::size_t index) const override;
        std::shared_ptr<Quanta::Texture> GetDepthTexture() const override; 

        void GetPixel(std::size_t index, void* data, std::size_t x, std::size_t y) override;

        std::size_t GetWidth() const override;
        std::size_t GetHeight() const override;

        std::uint32_t GetHandle() const;
    private:
        std::size_t width = 0;
        std::size_t height = 0;

        std::vector<std::shared_ptr<Quanta::Texture>> colorTextures;
        std::shared_ptr<Quanta::Texture> depthTexture;

        std::uint32_t handle = 0;
    };
}
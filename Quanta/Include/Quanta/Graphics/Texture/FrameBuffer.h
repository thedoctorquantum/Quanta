#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Texture.h"

namespace Quanta
{
    struct FrameBuffer 
    {
        struct AttachmentDescription
        {
            TexelFormat format = TexelFormat::Rgba8I;
            bool isSwapChainTarget = false;
        };

        struct Description
        {
            std::vector<AttachmentDescription> colorAttachments;
            AttachmentDescription depthAttachment;

            std::size_t width = 0;
            std::size_t height = 0;
        };

        static std::shared_ptr<FrameBuffer> Create(const Description& description); 

        virtual ~FrameBuffer() = default;

        virtual void Clear(const glm::vec4& color, float depth, std::int32_t stencil) = 0;
        
        virtual void GetPixel(std::size_t index, void* data, std::size_t x, std::size_t y) = 0;

        virtual std::shared_ptr<Texture> GetColorTexture(size_t index) const = 0;
        virtual std::shared_ptr<Texture> GetDepthTexture() const = 0; 
        
        virtual std::size_t GetWidth() const = 0;
        virtual std::size_t GetHeight() const = 0;
    };
}
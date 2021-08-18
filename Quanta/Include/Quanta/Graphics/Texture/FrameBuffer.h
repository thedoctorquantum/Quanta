#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Texture.h"

namespace Quanta
{
    class FrameBuffer 
    {
    public:
        struct AttachmentDescription final
        {
        public:
            TexelFormat format = TexelFormat::Rgba8I;
            bool isSwapChainTarget = false;
        };

        struct Description final
        {
        public:
            std::vector<AttachmentDescription> attachments;

            size_t width = 0;
            size_t height = 0;
        };

        static std::shared_ptr<FrameBuffer> Create(const Description& description); 

        virtual ~FrameBuffer() = default;

        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;

        virtual void ClearTexture(size_t index, const glm::vec4& color) = 0;

        virtual std::shared_ptr<Texture> GetTexture(size_t index) const = 0;
    };
}
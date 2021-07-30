#pragma once

#include "../../../Quanta/Graphics/Rendering/Texture/Sampler2D.h"

namespace Quanta
{
    class OpenGLSampler2D final : public Sampler2D 
    {
    public:
        OpenGLSampler2D(const std::shared_ptr<Texture2D>& texture);
        ~OpenGLSampler2D();

        OpenGLSampler2D(const Sampler2D&) = delete;
        OpenGLSampler2D& operator=(const Sampler2D&) = delete;

        const std::shared_ptr<Texture2D>& GetTexture() const override;

        uint32_t GetHandle() const;
    private:
        std::shared_ptr<Texture2D> texture = nullptr;
        
        uint32_t handle = 0;
    };
}
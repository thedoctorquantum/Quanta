#pragma once

#include <Quanta/Graphics/Texture/Sampler.h>

namespace Quanta
{
    class OpenGLSampler final : public Sampler
    {
    public:
        OpenGLSampler(const std::shared_ptr<Texture>& texture);
        ~OpenGLSampler();

        const std::shared_ptr<Texture>& GetTexture() const override; 
        
        FilterMode GetMagnification() const override;
        void SetMagnification(FilterMode) override;
        
        FilterMode GetMinification() const override;
        void SetMinification(FilterMode) override;

        WrapMode GetWrapModeX() const override;
        void SetWrapModeX(WrapMode) override;

        WrapMode GetWrapModeY() const override;
        void SetWrapModeY(WrapMode) override;

        WrapMode GetWrapModeZ() const override;
        void SetWrapModeZ(WrapMode) override;

        uint32_t GetHandle() const;
    private:
        std::shared_ptr<Texture> texture = nullptr;
        
        FilterMode magnifiaction = FilterMode::Linear;
        FilterMode minification = FilterMode::Linear;

        WrapMode wrapModeX = WrapMode::Repeat;
        WrapMode wrapModeY = WrapMode::Repeat;
        WrapMode wrapModeZ = WrapMode::Repeat;

        uint32_t handle = 0;
    };
}
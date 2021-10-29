#pragma once

#include <Quanta/Graphics/Texture/Sampler.h>

namespace Quanta::OpenGL
{
    struct Sampler : public Quanta::Sampler
    {
        Sampler(const std::shared_ptr<Quanta::Texture>& texture);
        ~Sampler();

        const std::shared_ptr<Quanta::Texture>& GetTexture() const override; 
        
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

        U32 GetHandle() const;
    private:
        std::shared_ptr<Quanta::Texture> texture = nullptr;
        
        FilterMode magnifiaction = FilterMode::Linear;
        FilterMode minification = FilterMode::Linear;

        WrapMode wrapModeX = WrapMode::Repeat;
        WrapMode wrapModeY = WrapMode::Repeat;
        WrapMode wrapModeZ = WrapMode::Repeat;

        U32 handle = 0;
    };
}
#pragma once

#include <Quanta/Graphics/Rendering/Texture/Sampler3D.h>

namespace Quanta
{
    class OpenGLSampler3D final : public Sampler3D
    {
    public:
        OpenGLSampler3D(const std::shared_ptr<Texture3D>& texture);
        ~OpenGLSampler3D();

        const std::shared_ptr<Texture3D>& GetTexture() const override; 

        FilterMode GetMagnification() const override;
        void SetMagnification(FilterMode value) override;
        
        FilterMode GetMinification() const override;
        void SetMinification(FilterMode value) override;
        
        WrapMode GetWrapModeX() const override;
        void SetWrapModeX(WrapMode value) override;
        
        WrapMode GetWrapModeY() const override;
        void SetWrapModeY(WrapMode value) override;
        
        WrapMode GetWrapModeZ() const override;
        void SetWrapModeZ(WrapMode value) override;

        uint32_t GetHandle() const;
    private: 
        std::shared_ptr<Texture3D> texture = nullptr;
        
        FilterMode magnifiaction = FilterMode::Linear;
        FilterMode minification = FilterMode::Linear;
        
        WrapMode wrapModeX = WrapMode::Repeat;
        WrapMode wrapModeY = WrapMode::Repeat;
        WrapMode wrapModeZ = WrapMode::Repeat;

        uint32_t handle = 0;
    };
}
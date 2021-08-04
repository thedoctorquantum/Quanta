#pragma once

#include <Quanta/Graphics/Texture/SamplerCube.h>

namespace Quanta
{
    class OpenGLSamplerCube final : public SamplerCube
    {
    public:
        OpenGLSamplerCube(const std::shared_ptr<CubeMap>& texture);
        ~OpenGLSamplerCube();

        OpenGLSamplerCube(const OpenGLSamplerCube&) = delete;

        OpenGLSamplerCube& operator=(const OpenGLSamplerCube&) = delete;

        const std::shared_ptr<CubeMap>& GetTexture() const override;
        
        FilterMode GetMagnification() const override;
        void SetMagnification(FilterMode value) override;
        
        FilterMode GetMinification() const override;
        void SetMinification(FilterMode value) override;
        
        WrapMode GetWrapModeX() const override;
        void SetWrapModeX(WrapMode value) override;
        
        WrapMode GetWrapModeY() const override;
        void SetWrapModeY(WrapMode value) override;
        
        bool GetIsSeamless() const override;
        void SetIsSeamless(bool value) override;

        uint32_t GetHandle() const;
    private:
        std::shared_ptr<CubeMap> texture;

        FilterMode magnification = FilterMode::Linear;
        FilterMode minification = FilterMode::Linear;

        WrapMode wrapModeX = WrapMode::Repeat;
        WrapMode wrapModeY = WrapMode::Repeat;

        bool isSeamless = false;

        uint32_t handle = 0;
    };
};
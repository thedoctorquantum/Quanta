#pragma once

#include <Quanta/Graphics/Texture/Sampler2DArray.h>

namespace Quanta
{
    class OpenGLSampler2DArray final : public Sampler2DArray
    {
    public:
        OpenGLSampler2DArray(const std::shared_ptr<Texture2DArray>& texture);
        ~OpenGLSampler2DArray(); 

        OpenGLSampler2DArray(const OpenGLSampler2DArray&) = delete;
        
        OpenGLSampler2DArray& operator=(const OpenGLSampler2DArray&) = delete;

        const std::shared_ptr<Texture2DArray>& GetTexture() const override;
    
        FilterMode GetMagnification() const override;
        void SetMagnification(FilterMode value) override;
        
        FilterMode GetMinification() const override;
        void SetMinification(FilterMode value) override;

        WrapMode GetWrapModeX() const override;
        void SetWrapModeX(WrapMode value) override;

        WrapMode GetWrapModeY() const override;
        void SetWrapModeY(WrapMode value) override;
        
        uint32_t GetHandle() const;
    private:
        std::shared_ptr<Texture2DArray> texture = nullptr;
        
        FilterMode magnifiaction = FilterMode::Linear;
        FilterMode minification = FilterMode::Linear;

        WrapMode wrapModeX = WrapMode::Repeat;
        WrapMode wrapModeY = WrapMode::Repeat;

        uint32_t handle = 0;
    };
}
#pragma once

#include <Quanta/Graphics/Rendering/Shader/ShaderModule.h>

namespace Quanta
{
    class OpenGLShaderModule final : public ShaderModule
    {
    public:
        OpenGLShaderModule(ShaderType type, const std::string& source);
        ~OpenGLShaderModule();

        OpenGLShaderModule(const OpenGLShaderModule&) = delete;

        OpenGLShaderModule& operator=(const OpenGLShaderModule&) = delete;
        
        uint32_t GetHandle() const;
    private:
        uint32_t handle;
    };
}
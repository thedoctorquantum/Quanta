#pragma once

#include <Quanta/Graphics/Shader/ShaderModule.h>

namespace Quanta::OpenGL
{
    struct ShaderModule : public Quanta::ShaderModule
    {
        ShaderModule(ShaderType type, const std::string& source);
        ~ShaderModule();

        ShaderModule(const ShaderModule&) = delete;

        ShaderModule& operator=(const ShaderModule&) = delete;
                
        U32 handle = 0;
    };
}
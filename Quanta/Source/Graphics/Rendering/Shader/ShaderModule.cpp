#include <iostream>
#include <Quanta/Graphics/Rendering/Shader/ShaderModule.h>
#include <Quanta/Graphics/Rendering/GraphicsDevice.h>

#include "../../../Platform/Rendering/OpenGL/OpenGLShaderModule.h"

namespace Quanta
{
    std::shared_ptr<ShaderModule> ShaderModule::Create(ShaderType type, const std::string& source)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLShaderModule>(type, source);
        }

        return nullptr;
    }

    ShaderModule::~ShaderModule() = default;
}
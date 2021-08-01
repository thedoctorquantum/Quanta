#include <iostream>

#include "ShaderModule.h"
#include "../GraphicsDevice.h"
#include "../../../../Platform/Rendering/OpenGL/OpenGLShaderModule.h"

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
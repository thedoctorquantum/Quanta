#include <Quanta/Graphics/Shader/ShaderModule.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/ShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<ShaderModule> ShaderModule::Create(ShaderType type, const std::string& source)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGL::ShaderModule>(type, source);
        }

        return nullptr;
    }

    ShaderModule::~ShaderModule() = default;
}
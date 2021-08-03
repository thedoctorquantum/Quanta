#include <Quanta/Graphics/Texture/SamplerCube.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLSamplerCube.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<SamplerCube> SamplerCube::Create(const std::shared_ptr<CubeMap>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);

        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: return std::make_shared<OpenGLSamplerCube>(texture);
        }
        
        DEBUG_FAILURE_MESSAGE("GraphicsApi not supported!");

        return nullptr;
    }
    
    SamplerCube::~SamplerCube() = default;
}
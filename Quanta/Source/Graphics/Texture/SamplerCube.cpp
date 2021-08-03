#include <Quanta/Graphics/Texture/SamplerCube.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLSamplerCube.h"

namespace Quanta
{
    std::shared_ptr<SamplerCube> SamplerCube::Create(const std::shared_ptr<CubeMap>& texture)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: return std::make_shared<OpenGLSamplerCube>(texture);
        }

        return nullptr;
    }
    
    SamplerCube::~SamplerCube() = default;
}
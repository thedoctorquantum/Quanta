#include <Quanta/Graphics/Texture/Sampler3D.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLSampler3D.h"

namespace Quanta
{
    std::shared_ptr<Sampler3D> Sampler3D::Create(const std::shared_ptr<Texture3D>& texture)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: return std::make_shared<OpenGLSampler3D>(texture);
        }

        return nullptr;
    }
    
    Sampler3D::~Sampler3D() = default;
}
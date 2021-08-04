#include <Quanta/Graphics/Texture/Sampler2D.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLSampler2D.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Sampler2D> Sampler2D::Create(const std::shared_ptr<Texture2D>& texture) 
    {
        DEBUG_ASSERT(texture != nullptr);

        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);
        
        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLSampler2D>(texture);            
        }
        
        return nullptr;
    }
    
    Sampler2D::~Sampler2D() = default;
}
#include <Quanta/Graphics/Texture/Sampler.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/Sampler.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Sampler> Sampler::Create(const std::shared_ptr<Texture>& texture) 
    {
        DEBUG_ASSERT(texture != nullptr);

        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);
        
        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGL::Sampler>(texture);            
        }
        
        return nullptr;
    }
}
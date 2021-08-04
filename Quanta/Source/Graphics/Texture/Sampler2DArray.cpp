#include <Quanta/Graphics/Texture/Sampler2DArray.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLSampler2DArray.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Sampler2DArray> Sampler2DArray::Create(const std::shared_ptr<Texture2DArray>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);
        
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLSampler2DArray>(texture);
        }
        
        return nullptr;
    }

    Sampler2DArray::~Sampler2DArray() = default;
}
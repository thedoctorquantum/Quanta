#include <Quanta/Graphics/Texture/Texture2DArray.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLTexture2DArray.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<Texture2DArray> Texture2DArray::Create(size_t width, size_t height, size_t depth)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLTexture2DArray>(width, height, depth); 
        }
        
        DEBUG_FAILURE_MESSAGE("GraphicsAPI not supported for Texture2DArray");

        return nullptr;
    }

    Texture2DArray::~Texture2DArray() = default;
}
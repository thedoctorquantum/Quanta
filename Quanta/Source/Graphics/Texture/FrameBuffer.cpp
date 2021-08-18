#include <Quanta/Graphics/Texture/FrameBuffer.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Debugging/Validation.h"
#include "../../Platform/OpenGL/Texture.h"

namespace Quanta
{
    static std::shared_ptr<FrameBuffer> Create(const FrameBuffer::Description& description)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            return nullptr;
            //return std::make_shared<OpenGLTexture>(type, format, width, height, depth);
        }
        
        return nullptr;
    }
}
#include <Quanta/Graphics/Texture/FrameBuffer.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Debugging/Validation.h"
#include "../../Platform/OpenGL/FrameBuffer.h"

namespace Quanta
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBuffer::Description& description)
    {
        const auto api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch (api)
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGL::FrameBuffer>(description);
        }
        
        return nullptr;
    }
}
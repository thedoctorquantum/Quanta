#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/GraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<GraphicsBuffer> GraphicsBuffer::Create(const BufferUsage usage, const USize size)
    {
        const auto api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch (api)
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGL::GraphicsBuffer>(usage, size);
        }

        return nullptr;
    }
}
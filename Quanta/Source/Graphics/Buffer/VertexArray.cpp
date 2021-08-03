#include <Quanta/Graphics/Buffer/VertexArray.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLVertexArray.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
        }
        
        DEBUG_FAILURE_MESSAGE("GraphicsApi is not supported");

        return nullptr;
    }
    
    VertexArray::~VertexArray() = default;
}
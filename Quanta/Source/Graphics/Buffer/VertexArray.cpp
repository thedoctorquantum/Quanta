#include <Quanta/Graphics/Buffer/VertexArray.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLVertexArray.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        GraphicsApi api = GraphicsDevice::GetApi();
        
        DEBUG_ASSERT(api == GraphicsApi::OpenGL);
        
        switch(api)
        {
        case GraphicsApi::OpenGL: return std::make_shared<OpenGLVertexArray>();
        }

        return nullptr;
    }
    
    VertexArray::~VertexArray() = default;
}
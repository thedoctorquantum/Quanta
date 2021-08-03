#include <glad/glad.h>
#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLGraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<GraphicsBuffer> GraphicsBuffer::Create(BufferUsage usage, size_t size)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGLGraphicsBuffer>(usage, size);
        }
        
        DEBUG_FAILURE_MESSAGE("GraphicsApi is not currently implemented");
        
        return nullptr;
    }

    void GraphicsBuffer::Resize(GraphicsBuffer& buffer, size_t size)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            DEBUG_ASSERT(((OpenGLGraphicsBuffer*) &buffer) != nullptr);

            OpenGLGraphicsBuffer::Resize((OpenGLGraphicsBuffer&) buffer, size);

            break;
        default:
            DEBUG_FAILURE_MESSAGE("GraphicsApi is not currently implemented");
        }
    }
    
    GraphicsBuffer::~GraphicsBuffer() = default;
}
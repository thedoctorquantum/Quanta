#include <glad/glad.h>

#include "GraphicsBuffer.h"
#include "../GraphicsDevice.h"
#include "../../../../Platform/Rendering/OpenGL/OpenGLGraphicsBuffer.h"

namespace Quanta
{
    std::shared_ptr<GraphicsBuffer> GraphicsBuffer::Create(BufferUsage usage, size_t size)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            return std::make_shared<OpenGLGraphicsBuffer>(usage, size);
        }

        return nullptr;
    }

    void GraphicsBuffer::Resize(GraphicsBuffer& buffer, size_t size)
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL: 
            OpenGLGraphicsBuffer::Resize((OpenGLGraphicsBuffer&) buffer, size);

            break;
        }
    }
    
    GraphicsBuffer::~GraphicsBuffer() = default;
}
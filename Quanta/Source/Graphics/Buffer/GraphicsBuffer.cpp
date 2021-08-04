#include <glad/glad.h>
#include <Quanta/Graphics/Buffer/GraphicsBuffer.h>
#include <Quanta/Graphics/GraphicsDevice.h>

#include "../../Platform/OpenGL/OpenGLGraphicsBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<GraphicsBuffer> GraphicsBuffer::Create(BufferUsage usage, size_t size)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL: return std::make_shared<OpenGLGraphicsBuffer>(usage, size);
        }

        return nullptr;
    }
    
    void GraphicsBuffer::Resize(GraphicsBuffer& buffer, size_t size)
    {
        GraphicsApi api = GraphicsDevice::GetApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL: 
            OpenGLGraphicsBuffer* glBuffer = nullptr;
#if DEBUG
            glBuffer = dynamic_cast<OpenGLGraphicsBuffer*>(&buffer);

            DEBUG_ASSERT(glBuffer != nullptr);
#else
            glBuffer = reinterpret_cast<OpenGLGraphicsBuffer*>(&buffer);
#endif
            OpenGLGraphicsBuffer::Resize(*glBuffer, size);

            break;
        }
    }
    
    GraphicsBuffer::~GraphicsBuffer() = default;
}
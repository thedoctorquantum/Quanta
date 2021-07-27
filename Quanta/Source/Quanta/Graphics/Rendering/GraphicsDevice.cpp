#include "GraphicsDevice.h"
#include "../../../Platform/Rendering/OpenGL/OpenGLVertexArray.h"
#include "../../../Platform/Rendering/OpenGL/OpenGLGraphicsBuffer.h"
#include "../../../Platform/Rendering/OpenGL/OpenGLGraphicsDevice.h"

namespace Quanta
{
    struct State
    {
        GraphicsDevice* device = nullptr;
        GraphicsApi api = GraphicsApi::OpenGL;
    } static state;

    GraphicsApi GraphicsDevice::GetApi()
    {
        return state.api;
    }

    void GraphicsDevice::Initialize(GraphicsApi api)
    {
        state.api = api;

        switch(api)
        {
        case GraphicsApi::OpenGL:
            state.device = new OpenGLGraphicsDevice();

            break;
        }
    }
    
    void GraphicsDevice::DeInitialize()
    {
        delete state.device;
    }

    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        state.device->InternalClearBackBuffer(color, depth, stencil);
    }
    
    void GraphicsDevice::SetViewport(const glm::ivec4& viewport)
    {
        state.device->InternalSetViewport(viewport);
    }

    void GraphicsDevice::SetScissorViewport(const glm::ivec4& viewport)
    {
        state.device->InternalSetScissorViewport(viewport);
    }
    
    void GraphicsDevice::SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value)
    {
        state.device->InternalSetRasterPipeline(value);
    }

    void GraphicsDevice::SetVertexArray(const std::shared_ptr<VertexArray>& value)
    {
        state.device->InternalSetVertexArray(value);
    }
    
    void GraphicsDevice::BindTexture2D(const Texture2D& texture, uint32_t index)
    {
        state.device->InternalBindTexture2D(texture, index);
    }
    
    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        state.device->InternalDispatchDraw(command);
    }
}
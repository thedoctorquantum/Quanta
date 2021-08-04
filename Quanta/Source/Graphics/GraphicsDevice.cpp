#include <Quanta/Graphics/GraphicsDevice.h>

#include "../Platform/OpenGL/OpenGLVertexArray.h"
#include "../Platform/OpenGL/OpenGLGraphicsBuffer.h"
#include "../Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "../Debugging/Validation.h"

namespace Quanta
{
    struct State
    {
        GraphicsDevice* device = nullptr;
        GraphicsApi api = GraphicsApi::OpenGL;
    } static state;

    void GraphicsDevice::Initialize(const std::shared_ptr<Window>& window)
    {
        DEBUG_ASSERT(window != nullptr);

        GraphicsApi api = window->GetGraphicsApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            state.device = new OpenGLGraphicsDevice(window);

            break;
        }

        state.api = api;
    }
    
    void GraphicsDevice::DeInitialize()
    {
        delete state.device;
    }

    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalClearBackBuffer(color, depth, stencil);
    }
        
    void GraphicsDevice::SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalSetRasterPipeline(value);
    }

    void GraphicsDevice::SetVertexArray(const std::shared_ptr<VertexArray>& value)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalSetVertexArray(value);
    }
    
    void GraphicsDevice::BindSampler(const Sampler2D& sampler, size_t index)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalBindSampler(sampler, index);
    }
    
    void GraphicsDevice::BindSampler(const Sampler3D& sampler, size_t index)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalBindSampler(sampler, index);
    }
    
    void GraphicsDevice::BindSampler(const SamplerCube& sampler, size_t index)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalBindSampler(sampler, index);
    }

    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        DEBUG_ASSERT(state.device != nullptr);
    
        state.device->InternalDispatchDraw(command);
    }
    
    GraphicsApi GraphicsDevice::GetApi()
    {
        return state.api;
    }
}
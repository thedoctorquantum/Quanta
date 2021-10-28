#include <Quanta/Graphics/GraphicsDevice.h>

#include "Implementation.h"
#include "../Platform/OpenGL/Implementation.h"
#include "../Debugging/Validation.h"

namespace Quanta::GraphicsDevice
{
    struct
    {
        Implementation* implementation = nullptr;
        GraphicsApi api = GraphicsApi::OpenGL;
    } static state;
    
    void Create(const Window& window)
    {
        const auto api = window.GetGraphicsApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);
        
        state.api = api;

        switch (api)
        {
        case GraphicsApi::OpenGL:
            state.implementation = new OpenGL::Implementation(window);

            break;
        }
    }
    
    void Destroy()
    {
        delete state.implementation;
    }

    void ClearBackBuffer(const glm::vec4& color, const float depth, const std::int32_t stencil)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->ClearBackBuffer(color, depth, stencil);
    }
        
    void SetRasterPipeline(const RasterPipeline* const value)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->SetRasterPipeline(value);
    }

    void SetVertexArray(const VertexArray* const value)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->SetVertexArray(value);
    }
        
    void BindSampler(const Sampler* const sampler, const size_t index)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->BindSampler(sampler, index);
    }
    
    void DispatchDraw(const DrawCommand& command)
    {
        DEBUG_ASSERT(state.implementation != nullptr);
    
        state.implementation->DispatchDraw(command);
    }

    void SetViewport(const glm::uvec4& value)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->SetViewport(value);
    }

    void SetScissorViewport(const glm::uvec4& value)
    {
        DEBUG_ASSERT(state.implementation != nullptr);

        state.implementation->SetScissorViewport(value);
    }
    
    GraphicsApi GetApi()
    {
        return state.api;
    }
}
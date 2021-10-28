#pragma once

#include <Quanta/Graphics/GraphicsDevice.h>

namespace Quanta::GraphicsDevice
{
    class Implementation 
    {
    public:
        virtual ~Implementation() = default;

        virtual void ClearBackBuffer(const glm::vec4& color, float depth, std::int32_t stencil) = 0;

        virtual void SetRasterPipeline(const RasterPipeline*) = 0;
        virtual void SetVertexArray(const VertexArray*) = 0;

        virtual void BindSampler(const Sampler* sampler, std::size_t index) = 0;

        virtual void DispatchDraw(const DrawCommand& command) = 0;
        
        virtual void SetViewport(const glm::uvec4&) = 0;
        virtual void SetScissorViewport(const glm::uvec4&) = 0;
    };
}
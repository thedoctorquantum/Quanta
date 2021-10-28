#pragma once

#include <glad/glad.h>

#include "../../Graphics/Implementation.h"
#include "RasterPipeline.h"
#include "VertexArray.h"

namespace Quanta::OpenGL
{
    struct Implementation : public Quanta::GraphicsDevice::Implementation
    {
        Implementation(const Window& window);
        
        void ClearBackBuffer(const glm::vec4& color, float depth, std::int32_t stencil) override;

        void SetRasterPipeline(const Quanta::RasterPipeline*) override;
        void SetVertexArray(const Quanta::VertexArray*) override;

        void BindSampler(const Quanta::Sampler* sampler, std::size_t index) override;

        void DispatchDraw(const DrawCommand& command) override;

        void SetViewport(const glm::uvec4&) override;
        void SetScissorViewport(const glm::uvec4&) override;
    private:
        const VertexArray* vertexArray = nullptr;
        const RasterPipeline* rasterPipeline = nullptr;
        
        GLenum geometryLayout = 0;
        GLenum drawElementsType = 0;
        GLenum drawElementSize = 0;
    };
}
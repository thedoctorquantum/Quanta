#pragma once

#include <Quanta/Graphics/GraphicsDevice.h>
#include <glad/glad.h>

#include "RasterPipeline.h"
#include "VertexArray.h"

namespace Quanta::OpenGL
{
    class GraphicsDevice final : public Quanta::GraphicsDevice
    {
    public:
        GraphicsDevice(const Window& window);
    private:
        const VertexArray* vertexArray = nullptr;
        const RasterPipeline* rasterPipeline = nullptr;
        
        GLenum geometryLayout = 0;
        GLenum drawElementsType = 0;
        GLenum drawElementSize = 0;
        
        void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) override;
        
        void InternalSetRasterPipeline(const Quanta::RasterPipeline* value) override;
        void InternalSetVertexArray(const Quanta::VertexArray* value) override;
        
        void InternalBindSampler(const Quanta::Sampler* sampler, Size index) override;
        
        void InternalDispatchDraw(const DrawCommand& command) override;
    };
}
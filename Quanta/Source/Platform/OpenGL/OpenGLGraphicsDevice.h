#pragma once

#include <Quanta/Graphics/GraphicsDevice.h>
#include <glad/glad.h>

#include "OpenGLRasterPipeline.h"
#include "OpenGLVertexArray.h"

namespace Quanta
{
    class OpenGLGraphicsDevice final : public GraphicsDevice
    {
    public:
        OpenGLGraphicsDevice(const Window& window);
    private:
        const OpenGLVertexArray* vertexArray = nullptr;
        const OpenGLRasterPipeline* rasterPipeline = nullptr;
        
        GLenum geometryLayout = 0;
        GLenum drawElementsType = 0;
        GLenum drawElementSize = 0;
        
        void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) override;
        
        void InternalSetRasterPipeline(const RasterPipeline* value) override;
        void InternalSetVertexArray(const VertexArray* value) override;
        
        void InternalBindSampler(const Sampler* sampler, size_t index) override;
        
        void InternalDispatchDraw(const DrawCommand& command) override;
    };
}
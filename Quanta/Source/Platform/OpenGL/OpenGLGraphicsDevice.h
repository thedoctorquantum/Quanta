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
        OpenGLGraphicsDevice();
    private:
        OpenGLVertexArray* vertexArray = nullptr;
        OpenGLRasterPipeline* rasterPipeline = nullptr;

        GLenum geometryLayout = 0;
        
        uint32_t maxTextureSlots = 0;
        
        void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) override;
        
        void InternalSetRasterPipeline(const std::shared_ptr<RasterPipeline>& value) override;
        void InternalSetVertexArray(const std::shared_ptr<VertexArray>& value) override;
        
        void InternalBindSampler(const Sampler2D& sampler, size_t index) override;
        void InternalBindSampler(const Sampler3D& sampler, size_t index) override;
        void InternalBindSampler(const SamplerCube& sampler, size_t index) override;

        void InternalDispatchDraw(const DrawCommand& command) override;
    };
}
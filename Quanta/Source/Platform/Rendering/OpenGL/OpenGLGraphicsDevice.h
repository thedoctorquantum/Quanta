#pragma once

#include <Quanta/Graphics/Rendering/GraphicsDevice.h>
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
        
        void InternalBindSampler2D(const Sampler2D& sampler, uint32_t index) override;
        void InternalBindSampler3D(const Sampler3D& sampler, uint32_t index) override;

        void InternalDispatchDraw(const DrawCommand& command) override;
    };
}
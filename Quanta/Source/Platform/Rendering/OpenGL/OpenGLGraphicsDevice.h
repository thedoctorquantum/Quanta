#pragma once

#include <glad/glad.h>

#include "../../../Quanta/Graphics/Rendering/GraphicsDevice.h"
#include "OpenGLVertexArray.h"

namespace Quanta
{
    class OpenGLGraphicsDevice final : public GraphicsDevice
    {
    public:
        OpenGLGraphicsDevice();
    private:
        OpenGLVertexArray* vertexArray = nullptr;
        RasterPipeline* rasterPipeline = nullptr;

        GLenum geometryLayout = 0;
        GLenum indexType = 0;    
        
        void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) override;
        void InternalSetViewport(const glm::ivec4& value) override;
        void InternalSetScissorViewport(const glm::ivec4& value) override;

        void InternalSetRasterPipeline(const std::shared_ptr<RasterPipeline>& value) override;
        void InternalSetVertexArray(const std::shared_ptr<VertexArray>& value) override;

        void InternalBindTexture2D(const Texture2D& texture, uint32_t index) override;

        void InternalDispatchDraw(const DrawCommand& command) override;
    };
}
#pragma once

#include <glm/glm.hpp>

#include "Buffer/GraphicsBuffer.h"
#include "Buffer/VertexArray.h"
#include "Pipeline/RasterPipeline.h"
#include "DrawCommand.h"

namespace Quanta
{
    class GraphicsDevice final
    {
    public:
        GraphicsDevice() = delete; 
        ~GraphicsDevice() = delete;

        GraphicsDevice(const GraphicsDevice& other) = delete;
        GraphicsDevice(const GraphicsDevice&& other) = delete;

        static void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);
        static void Viewport(const glm::ivec4& viewport);

        static void SetPipeline(const std::shared_ptr<RasterPipeline>& pipeline);
        static void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);

        static void DispatchDraw(const DrawCommand& command);
    };
}
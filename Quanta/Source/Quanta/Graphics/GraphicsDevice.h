#pragma once

#include <glm/glm.hpp>
#include "Buffers/GraphicsBuffer.h"
#include "Buffers/VertexArray.h"
#include "RasterPipeline.h"

namespace Quanta
{
    struct DrawCommand final 
    {
        uint32_t Count = 0;
        uint32_t InstanceCount = 0;
        uint64_t IndexOffset = 0;
        uint32_t StartVertex = 0;
        uint32_t StartInstance = 1;
    };

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
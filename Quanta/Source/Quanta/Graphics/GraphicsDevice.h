#pragma once

#include <glm/glm.hpp>

#include "Buffer/GraphicsBuffer.h"
#include "Buffer/VertexArray.h"
#include "Pipeline/RasterPipeline.h"
#include "DrawCommand.h"

namespace Quanta::GraphicsDevice
{
    void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);
    void Viewport(const glm::ivec4& viewport);

    void SetRasterPipeline(const std::shared_ptr<RasterPipeline>& pipeline);
    void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);

    void DispatchDraw(const DrawCommand& command);
}
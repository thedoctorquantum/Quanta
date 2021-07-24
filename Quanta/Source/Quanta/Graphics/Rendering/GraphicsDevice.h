#pragma once

#include <glm/glm.hpp>

#include "Pipeline/RasterPipeline.h"
#include "Buffer/VertexArray.h"
#include "DrawCommand.h"

namespace Quanta::GraphicsDevice
{
    void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);
    void SetViewport(const glm::ivec4& value);
    void SetScissorViewport(const glm::ivec4& value);

    void SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value);
    void SetVertexArray(const std::shared_ptr<VertexArray>& value);

    void DispatchDraw(const DrawCommand& command);
}
#pragma once

#include <glm/glm.hpp>

#include "Pipeline/RasterPipeline.h"
#include "Buffer/VertexArray.h"
#include "Texture/Sampler.h"
#include "GraphicsApi.h"
#include "DrawCommand.h"
#include "../Core/Windowing/Window.h"

namespace Quanta::GraphicsDevice
{
    void Create(const Window& window);
    void Destroy();

    void ClearBackBuffer(const glm::vec4& color, float depth, std::int32_t stencil);

    void SetRasterPipeline(const RasterPipeline*);
    void SetVertexArray(const VertexArray*);

    void BindSampler(const Sampler* sampler, std::size_t index);

    void DispatchDraw(const DrawCommand& command);

    void SetViewport(const glm::uvec4&);
    void SetScissorViewport(const glm::uvec4&);

    GraphicsApi GetApi();
}
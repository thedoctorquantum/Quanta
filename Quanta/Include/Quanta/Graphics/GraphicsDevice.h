#pragma once

#include <glm/glm.hpp>

#include "Pipeline/RasterPipeline.h"
#include "Buffer/VertexArray.h"
#include "Texture/Sampler.h"
#include "GraphicsApi.h"
#include "DrawCommand.h"
#include "../Core/Windowing/Window.h"

namespace Quanta
{
    class GraphicsDevice
    {
    public:
        static void Initialize(const Window* window);
        static void DeInitialize();

        static void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);

        static void SetRasterPipeline(const RasterPipeline* value);
        static void SetVertexArray(const VertexArray* value);

        static void BindSampler(const Sampler* sampler, size_t index);

        static void DispatchDraw(const DrawCommand& command);

        static GraphicsApi GetApi();
    private:
        virtual void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) = 0;

        virtual void InternalSetRasterPipeline(const RasterPipeline* value) = 0;
        virtual void InternalSetVertexArray(const VertexArray* value) = 0;
        
        virtual void InternalBindSampler(const Sampler* sampler, size_t index) = 0;
        
        virtual void InternalDispatchDraw(const DrawCommand& command) = 0;
    };
}
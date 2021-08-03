#pragma once

#include <glm/glm.hpp>

#include "Pipeline/RasterPipeline.h"
#include "Buffer/VertexArray.h"
#include "Texture/Sampler2D.h"
#include "Texture/Sampler3D.h"
#include "Texture/SamplerCube.h"
#include "GraphicsApi.h"
#include "DrawCommand.h"

namespace Quanta
{
    class GraphicsDevice
    {
    public:
        static GraphicsApi GetApi();

        static void Initialize(GraphicsApi api);
        static void DeInitialize();

        static void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);

        static void SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value);
        static void SetVertexArray(const std::shared_ptr<VertexArray>& value);

        static void BindSampler(const Sampler2D& sampler, size_t index);
        static void BindSampler(const Sampler3D& sampler, size_t index);
        static void BindSampler(const SamplerCube& sampler, size_t index);

        static void DispatchDraw(const DrawCommand& command);
    private:
        virtual void InternalClearBackBuffer(const glm::vec4& color, float depth, int stencil) = 0;

        virtual void InternalSetRasterPipeline(const std::shared_ptr<RasterPipeline>& value) = 0;
        virtual void InternalSetVertexArray(const std::shared_ptr<VertexArray>& value) = 0;
        
        virtual void InternalBindSampler(const Sampler2D& sampler, size_t index) = 0;
        virtual void InternalBindSampler(const Sampler3D& sampler, size_t index) = 0;
        virtual void InternalBindSampler(const SamplerCube& sampler, size_t index) = 0;

        virtual void InternalDispatchDraw(const DrawCommand& command) = 0;
    };
}
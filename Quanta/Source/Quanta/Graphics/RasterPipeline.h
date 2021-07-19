#pragma once

#include "RasterPipelineDescription.h"

namespace Quanta
{
    class RasterPipeline final
    {
    public:
        RasterPipeline(const std::shared_ptr<RasterPipelineDescription>& description);
        ~RasterPipeline();

        const std::shared_ptr<GraphicsBuffer>& GetModule(uint32_t index) const;

        const std::vector<std::shared_ptr<ShaderModule>>& GetShaderModules() const;
        const std::vector<std::shared_ptr<GraphicsBuffer>>& GetUniformBuffers() const;

        PolygonFillMode GetPolygonFillMode() const;
        bool IsDepthTestingEnabled() const;
        bool IsScissorTestingEnabled() const;
        bool IsBlendingEnabled() const;
        FaceCullMode GetFaceCullMode() const;
        GeometryLayout GetGeometryLayout() const;
        GeometryWinding GetGeometryWinding() const;

        uint32_t GetHandle() const;
    private:
        uint32_t handle;

        std::shared_ptr<RasterPipelineDescription> description;
    };
}
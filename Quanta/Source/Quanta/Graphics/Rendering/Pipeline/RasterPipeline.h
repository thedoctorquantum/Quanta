#pragma once

#include <memory>
#include <vector>

#include "../Buffer/GraphicsBuffer.h"
#include "../Shader/ShaderModule.h"
#include "BlendFactor.h"
#include "BlendMode.h"
#include "DepthTestMode.h"
#include "FaceCullMode.h"
#include "GeometryLayout.h"
#include "GeometryWinding.h"
#include "PolygonFillMode.h"
#include "RasterPipelineDescription.h"

namespace Quanta
{
    class RasterPipeline final
    {
    public:
        explicit RasterPipeline(const RasterPipelineDescription& description);
        ~RasterPipeline();

        const std::vector<std::shared_ptr<ShaderModule>>& GetShaderModules() const;
        const std::vector<std::shared_ptr<GraphicsBuffer>>& GetUniformBuffers() const;

        PolygonFillMode GetPolygonFillMode() const;
        void SetPolygonFillMode(PolygonFillMode value);

        DepthTestMode GetDepthTestMode() const;
        void SetDepthTestMode(DepthTestMode value);

        bool GetEnableDepthWriting() const;
        void SetEnableDepthWriting(bool value);
        
        bool GetEnableScissorTesting() const;
        void SetEnableScissorTesting(bool value);

        BlendMode GetBlendMode() const;
        void SetBlendMode(BlendMode value);
        
        BlendFactor GetBlendFactor() const;
        void SetBlendFactor(BlendFactor blendFactor);
        
        FaceCullMode GetFaceCullMode() const;
        void SetFaceCullMode(FaceCullMode value);
        
        GeometryLayout GetGeometryLayout() const;
        void SetGeometryLayout(GeometryLayout value);
        
        GeometryWinding GetGeometryWinding() const;
        void SetGeometryWinding(GeometryWinding value);

        uint32_t GetHandle() const;
    private:
        uint32_t handle;
        
        std::vector<std::shared_ptr<ShaderModule>> shaderModules;
        std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers;
        
        PolygonFillMode polygonFillMode = PolygonFillMode::Solid;
        DepthTestMode depthTestMode = DepthTestMode::None;
        bool enableDepthWriting = false;
        bool enableScissorTesting = false;
        BlendMode blendMode = BlendMode::None;
        BlendFactor blendFactor = BlendFactor::One;
        FaceCullMode faceCullMode = FaceCullMode::None;
        GeometryLayout geometryLayout = GeometryLayout::Triangle;
        GeometryWinding geometryWinding = GeometryWinding::CounterClockwise;
    };
}
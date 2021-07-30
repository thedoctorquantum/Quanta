#pragma once

#include <memory>
#include <glm/glm.hpp>

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
    class RasterPipeline
    {
    public:
        static std::shared_ptr<RasterPipeline> Create(const RasterPipelineDescription& description);

        virtual const std::shared_ptr<ShaderModule>& GetShaderModule(size_t index) const = 0;
        virtual const std::shared_ptr<GraphicsBuffer>& GetUniformBuffer(size_t index) const = 0;

        virtual size_t GetShaderModuleCount() const = 0;
        virtual size_t GetUniformBufferCount() const = 0;

        virtual const glm::uvec4& GetViewport() const = 0;
        virtual void SetViewport(const glm::uvec4& value) = 0;

        virtual const glm::uvec4& GetScissorViewport() const = 0;
        virtual void SetScissorViewport(const glm::uvec4& value) = 0;

        virtual PolygonFillMode GetPolygonFillMode() const = 0;
        virtual void SetPolygonFillMode(PolygonFillMode value) = 0;

        virtual DepthTestMode GetDepthTestMode() const = 0;
        virtual void SetDepthTestMode(DepthTestMode value) = 0;

        virtual bool GetEnableDepthWriting() const = 0;
        virtual void SetEnableDepthWriting(bool value) = 0;
        
        virtual bool GetEnableScissorTesting() const = 0;
        virtual void SetEnableScissorTesting(bool value) = 0;

        virtual BlendMode GetBlendMode() const = 0;
        virtual void SetBlendMode(BlendMode value) = 0;
        
        virtual BlendFactor GetBlendFactor() const = 0;
        virtual void SetBlendFactor(BlendFactor blendFactor) = 0;
        
        virtual FaceCullMode GetFaceCullMode() const = 0;
        virtual void SetFaceCullMode(FaceCullMode value) = 0;
        
        virtual GeometryLayout GetGeometryLayout() const = 0;
        virtual void SetGeometryLayout(GeometryLayout value) = 0;
        
        virtual GeometryWinding GetGeometryWinding() const = 0;
        virtual void SetGeometryWinding(GeometryWinding value) = 0;
    };
}
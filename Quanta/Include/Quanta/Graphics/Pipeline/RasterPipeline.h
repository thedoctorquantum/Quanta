#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../Buffer/GraphicsBuffer.h"
#include "../Shader/ShaderModule.h"
#include "../Texture/FrameBuffer.h"
#include "BlendFactor.h"
#include "BlendMode.h"
#include "DepthTestMode.h"
#include "FaceCullMode.h"
#include "GeometryLayout.h"
#include "GeometryWinding.h"
#include "PolygonFillMode.h"

namespace Quanta
{
    class RasterPipeline
    {
    public:
        struct Description final
        {
        public:
            std::vector<std::shared_ptr<ShaderModule>> shaderModules;
            std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers;
            std::vector<std::shared_ptr<GraphicsBuffer>> storageBuffers;

            std::shared_ptr<FrameBuffer> frameBuffer = nullptr;
        };
    
        static std::shared_ptr<RasterPipeline> Create(const Description& description);

        virtual ~RasterPipeline() = default;

        virtual const std::shared_ptr<ShaderModule>& GetShaderModule(USize index) const = 0;
        virtual const std::shared_ptr<GraphicsBuffer>& GetUniformBuffer(USize index) const = 0;
        virtual const std::shared_ptr<GraphicsBuffer>& GetStorageBuffer(USize index) const = 0;

        virtual const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const = 0;
        
        virtual USize GetShaderModuleCount() const = 0;
        virtual USize GetUniformBufferCount() const = 0;
        virtual USize GetStorageBufferCount() const = 0;

        virtual const glm::uvec4& GetViewport() const = 0;
        virtual void SetViewport(const glm::uvec4&) = 0;

        virtual const glm::uvec4& GetScissorViewport() const = 0;
        virtual void SetScissorViewport(const glm::uvec4&) = 0;

        virtual PolygonFillMode GetPolygonFillMode() const = 0;
        virtual void SetPolygonFillMode(PolygonFillMode) = 0;

        virtual DepthTestMode GetDepthTestMode() const = 0;
        virtual void SetDepthTestMode(DepthTestMode) = 0;

        virtual bool GetEnableDepthWriting() const = 0;
        virtual void SetEnableDepthWriting(bool) = 0;
        
        virtual bool GetEnableScissorTesting() const = 0;
        virtual void SetEnableScissorTesting(bool) = 0;

        virtual BlendMode GetBlendMode() const = 0;
        virtual void SetBlendMode(BlendMode) = 0;
        
        virtual BlendFactor GetBlendFactor() const = 0;
        virtual void SetBlendFactor(BlendFactor) = 0;
        
        virtual FaceCullMode GetFaceCullMode() const = 0;
        virtual void SetFaceCullMode(FaceCullMode) = 0;
        
        virtual GeometryLayout GetGeometryLayout() const = 0;
        virtual void SetGeometryLayout(GeometryLayout) = 0;
        
        virtual GeometryWinding GetGeometryWinding() const = 0;
        virtual void SetGeometryWinding(GeometryWinding) = 0;
    };
}
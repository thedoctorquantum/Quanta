#pragma once

#include <Quanta/Graphics/Pipeline/RasterPipeline.h>

namespace Quanta
{
    class OpenGLRasterPipeline final : public RasterPipeline
    {
    public:
        explicit OpenGLRasterPipeline(const RasterPipelineDescription& description);
        ~OpenGLRasterPipeline();

        OpenGLRasterPipeline(const OpenGLRasterPipeline&) = delete;

        OpenGLRasterPipeline& operator=(const OpenGLRasterPipeline&) = delete; 

        const std::shared_ptr<ShaderModule>& GetShaderModule(size_t index) const override;
        const std::shared_ptr<GraphicsBuffer>& GetUniformBuffer(size_t index) const override;
        
        size_t GetShaderModuleCount() const override;
        size_t GetUniformBufferCount() const override;

        const glm::uvec4& GetViewport() const override;
        void SetViewport(const glm::uvec4& value) override;

        const glm::uvec4& GetScissorViewport() const override;
        void SetScissorViewport(const glm::uvec4& value) override;

        PolygonFillMode GetPolygonFillMode() const override;
        void SetPolygonFillMode(PolygonFillMode value) override;

        DepthTestMode GetDepthTestMode() const override;
        void SetDepthTestMode(DepthTestMode value) override;

        bool GetEnableDepthWriting() const override;
        void SetEnableDepthWriting(bool value) override;
        
        bool GetEnableScissorTesting() const override;
        void SetEnableScissorTesting(bool value) override;

        BlendMode GetBlendMode() const override;
        void SetBlendMode(BlendMode value) override;
        
        BlendFactor GetBlendFactor() const override;
        void SetBlendFactor(BlendFactor blendFactor) override;
        
        FaceCullMode GetFaceCullMode() const override;
        void SetFaceCullMode(FaceCullMode value) override;
        
        GeometryLayout GetGeometryLayout() const override;
        void SetGeometryLayout(GeometryLayout value) override;
        
        GeometryWinding GetGeometryWinding() const override;
        void SetGeometryWinding(GeometryWinding value) override;

        uint32_t GetHandle() const;
    private:
        uint32_t handle = 0;

        size_t instanceId = 0;
        
        std::vector<std::shared_ptr<ShaderModule>> shaderModules;
        std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers;
        
        glm::uvec4 viewport = { };
        glm::uvec4 scissorViewport = { };

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
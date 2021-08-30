#pragma once

#include <Quanta/Graphics/Pipeline/RasterPipeline.h>

namespace Quanta::OpenGL
{
    class RasterPipeline final : public Quanta::RasterPipeline
    {
    public:
        explicit RasterPipeline(const RasterPipeline::Description& description);
        ~RasterPipeline();

        RasterPipeline(const RasterPipeline&) = delete;

        RasterPipeline& operator=(const RasterPipeline&) = delete; 

        const std::shared_ptr<Quanta::ShaderModule>& GetShaderModule(USize index) const override;
        const std::shared_ptr<Quanta::GraphicsBuffer>& GetUniformBuffer(USize index) const override;
        const std::shared_ptr<Quanta::GraphicsBuffer>& GetStorageBuffer(USize index) const override;
        
        const std::shared_ptr<Quanta::FrameBuffer>& GetFrameBuffer() const override;

        USize GetShaderModuleCount() const override;
        USize GetUniformBufferCount() const override;
        USize GetStorageBufferCount() const override;

        const glm::uvec4& GetViewport() const override;
        void SetViewport(const glm::uvec4&) override;

        const glm::uvec4& GetScissorViewport() const override;
        void SetScissorViewport(const glm::uvec4&) override;

        PolygonFillMode GetPolygonFillMode() const override;
        void SetPolygonFillMode(PolygonFillMode) override;

        DepthTestMode GetDepthTestMode() const override;
        void SetDepthTestMode(DepthTestMode) override;

        bool GetEnableDepthWriting() const override;
        void SetEnableDepthWriting(bool) override;
        
        bool GetEnableScissorTesting() const override;
        void SetEnableScissorTesting(bool) override;

        BlendMode GetBlendMode() const override;
        void SetBlendMode(BlendMode) override;
        
        BlendFactor GetBlendFactor() const override;
        void SetBlendFactor(BlendFactor) override;
        
        FaceCullMode GetFaceCullMode() const override;
        void SetFaceCullMode(FaceCullMode) override;
        
        GeometryLayout GetGeometryLayout() const override;
        void SetGeometryLayout(GeometryLayout) override;
        
        GeometryWinding GetGeometryWinding() const override;
        void SetGeometryWinding(GeometryWinding) override;

        U32 GetHandle() const;
    private:
        U32 handle = 0;
        
        std::vector<std::shared_ptr<Quanta::ShaderModule>> shaderModules;
        std::vector<std::shared_ptr<Quanta::GraphicsBuffer>> uniformBuffers;
        std::vector<std::shared_ptr<Quanta::GraphicsBuffer>> storageBuffers;

        std::shared_ptr<Quanta::FrameBuffer> frameBuffer = nullptr;
        
        glm::uvec4 viewport = glm::uvec4(0.0f);
        glm::uvec4 scissorViewport = glm::uvec4(0.0f);

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
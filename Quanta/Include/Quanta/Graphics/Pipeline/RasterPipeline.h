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
    struct RasterPipeline
    {
        struct Description
        {
            std::shared_ptr<ShaderModule> vertexShader;
            std::shared_ptr<ShaderModule> fragmentShader;
            std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers;
            std::vector<std::shared_ptr<GraphicsBuffer>> storageBuffers;

            std::shared_ptr<FrameBuffer> frameBuffer = nullptr;
        };
    
        static std::shared_ptr<RasterPipeline> Create(const Description& description);

        virtual ~RasterPipeline() = default;

        std::shared_ptr<ShaderModule> vertexShader;
        std::shared_ptr<ShaderModule> fragmentShader;

        std::shared_ptr<FrameBuffer> framebuffer;

        std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers;
        std::vector<std::shared_ptr<GraphicsBuffer>> storageBuffers;
        
        PolygonFillMode polygonFillMode = PolygonFillMode::Solid;
        DepthTestMode depthTestMode = DepthTestMode::Always;
        bool enableDepthWriting = false;
        bool enableScissorTesting = false;
        BlendMode blendMode = BlendMode::None;
        BlendFactor blendFactor = BlendFactor::Zero;
        FaceCullMode faceCullMode = FaceCullMode::None;
        GeometryLayout geometryLayout = GeometryLayout::Triangle;
        GeometryWinding geometryWinding = GeometryWinding::CounterClockwise;
    };
}
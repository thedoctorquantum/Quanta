#pragma once

#include <memory>
#include <vector>

#include "../Shader/ShaderModule.h"
#include "../Buffer/GraphicsBuffer.h"
#include "PolygonFillMode.h"
#include "FaceCullMode.h"
#include "GeometryLayout.h"
#include "GeometryWinding.h"

namespace Quanta
{
    struct RasterPipelineDescription final
    {
        PolygonFillMode FillMode = PolygonFillMode::Solid;
        bool EnableDepthTesting = false;
        bool EnableScissorTesting = false;
        bool EnableBlending = false;
        FaceCullMode CullMode = FaceCullMode::None;
        GeometryLayout Layout = GeometryLayout::Triangle;
        GeometryWinding Winding = GeometryWinding::CounterClockwise;
        
        std::vector<std::shared_ptr<GraphicsBuffer>> UniformBuffers;
        std::vector<std::shared_ptr<ShaderModule>> ShaderModules;
    };   
}
#include <iostream>
#include <glad/glad.h>

#include "RasterPipeline.h"

namespace Quanta
{
    RasterPipeline::RasterPipeline(const RasterPipelineDescription& description)
    {
        this->shaderModules = description.ShaderModules;
        this->uniformBuffers = description.UniformBuffers;
        
        handle = glCreateProgram();

        for(int i = 0; i < shaderModules.size(); i++)
        {
            const std::shared_ptr<ShaderModule>& shader = shaderModules[i];
            
            glAttachShader(handle, shader->GetHandle());
        }
    
        glLinkProgram(handle);

        int success;

        glGetProgramiv(handle, GL_LINK_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            std::cout << "Shader program " << handle << " could not link: \n" << infoLog << '\n'; 
        }

        for(size_t i = 0; i < shaderModules.size(); i++)
        {
            const std::shared_ptr<ShaderModule>& shader = shaderModules[i];
            
            glDetachShader(handle, shader->GetHandle());
        }
    }
    
    RasterPipeline::~RasterPipeline()
    {
        glDeleteProgram(handle);
    }
    
    const std::vector<std::shared_ptr<ShaderModule>>& RasterPipeline::GetShaderModules() const
    {
        return shaderModules;
    }
    
    const std::vector<std::shared_ptr<GraphicsBuffer>>& RasterPipeline::GetUniformBuffers() const
    {
        return uniformBuffers;
    }

    PolygonFillMode RasterPipeline::GetPolygonFillMode() const
    {
        return polygonFillMode;
    }

    void RasterPipeline::SetPolygonFillMode(PolygonFillMode value)
    {
        polygonFillMode = value;
    }

    DepthTestMode RasterPipeline::GetDepthTestMode() const
    {
        return depthTestMode;
    }

    void RasterPipeline::SetDepthTestMode(DepthTestMode value)
    {
        depthTestMode = value;
    }

    bool RasterPipeline::GetEnableDepthWriting() const
    {
        return enableDepthWriting;
    }

    void RasterPipeline::SetEnableDepthWriting(bool value)
    {
        enableDepthWriting = value;
    }

    bool RasterPipeline::GetEnableScissorTesting() const
    {
        return enableScissorTesting;
    }

    void RasterPipeline::SetEnableScissorTesting(bool value)
    {
        enableScissorTesting = value;
    }

    BlendMode RasterPipeline::GetBlendMode() const
    {
        return blendMode;
    }

    void RasterPipeline::SetBlendMode(BlendMode value)
    {
        blendMode = value;
    }

    BlendFactor RasterPipeline::GetBlendFactor() const
    {
        return blendFactor;
    }
    
    void RasterPipeline::SetBlendFactor(BlendFactor value)
    {
        blendFactor = value;
    }

    FaceCullMode RasterPipeline::GetFaceCullMode() const
    {
        return faceCullMode;
    }

    void RasterPipeline::SetFaceCullMode(FaceCullMode value)
    {
        faceCullMode = value;
    }

    GeometryLayout RasterPipeline::GetGeometryLayout() const
    {
        return geometryLayout;
    }

    void RasterPipeline::SetGeometryLayout(GeometryLayout value)
    {
        geometryLayout = value;
    }

    GeometryWinding RasterPipeline::GetGeometryWinding() const
    {
        return geometryWinding;
    }

    void RasterPipeline::SetGeometryWinding(GeometryWinding value)
    {
        geometryWinding = value;
    }

    uint32_t RasterPipeline::GetHandle() const
    {
        return handle;
    }
}
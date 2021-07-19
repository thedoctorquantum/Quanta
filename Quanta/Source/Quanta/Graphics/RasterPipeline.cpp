#include "RasterPipeline.h"

#include <iostream>
#include <glad/glad.h>

namespace Quanta
{
    RasterPipeline::RasterPipeline(const std::shared_ptr<RasterPipelineDescription>& description)
    {
        this->description = description;

        handle = glCreateProgram();

        for(int i = 0; i < description->ShaderModules.size(); i++)
        {
            const std::shared_ptr<ShaderModule>& shader = description->ShaderModules[i];
            
            glAttachShader(handle, shader->GetHandle());
        }
    
        glLinkProgram(handle);

        int success;

        glGetProgramiv(handle, GL_LINK_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            std::cout << "Shader program " << handle << " could not link: \n" << infoLog << std::endl; 
        }

        for(int i = 0; i < description->ShaderModules.size(); i++)
        {
            const std::shared_ptr<ShaderModule>& shader = description->ShaderModules[i];
            
            glDetachShader(handle, shader->GetHandle());
        }
    }
    
    RasterPipeline::~RasterPipeline()
    {
        glDeleteProgram(handle);
    }
    
    const std::vector<std::shared_ptr<ShaderModule>>& RasterPipeline::GetShaderModules() const
    {
        return description->ShaderModules;
    }
    
    const std::vector<std::shared_ptr<GraphicsBuffer>>& RasterPipeline::GetUniformBuffers() const
    {
        return description->UniformBuffers;
    }

    PolygonFillMode RasterPipeline::GetPolygonFillMode() const
    {
        return description->FillMode;
    }

    bool RasterPipeline::IsDepthTestingEnabled() const
    {
        return description->EnableDepthTesting;
    }

    bool RasterPipeline::IsScissorTestingEnabled() const
    {
        return description->EnableScissorTesting;
    }

    bool RasterPipeline::IsBlendingEnabled() const
    {
        return description->EnableBlending;
    }

    FaceCullMode RasterPipeline::GetFaceCullMode() const
    {
        return description->CullMode;
    }

    GeometryLayout RasterPipeline::GetGeometryLayout() const
    {
        return description->Layout;
    }

    GeometryWinding RasterPipeline::GetGeometryWinding() const
    {
        return description->Winding;
    }

    uint32_t RasterPipeline::GetHandle() const
    {
        return handle;
    }
}
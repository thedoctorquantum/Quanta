#include <glad/glad.h>

#include "RasterPipeline.h"
#include "ShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenGL
{
    RasterPipeline::RasterPipeline(const RasterPipeline::Description& description)
    {
        DEBUG_ASSERT(description.ShaderModules.size() >= 2);
        
        this->shaderModules = description.ShaderModules;
        this->uniformBuffers = description.UniformBuffers;
        this->storageBuffers = description.StorageBuffers;
        
        handle = glCreateProgram();

        DEBUG_ASSERT(handle != 0);

        for (const std::shared_ptr<Quanta::ShaderModule>& shader : shaderModules)
        {
            const ShaderModule* glShader = nullptr;

            if constexpr (DEBUG)
            {
                glShader = dynamic_cast<const ShaderModule*>(shader.get());

                DEBUG_ASSERT(glShader != nullptr);
            }
            else
            {
                glShader = static_cast<const ShaderModule*>(shader.get());
            }
            
            glAttachShader(handle, glShader->GetHandle());
        }
    
        glLinkProgram(handle);

        if constexpr (DEBUG)
        {
            int success;

            glGetProgramiv(handle, GL_LINK_STATUS, &success);

            if(!success)
            {
                char infoLog[512];

                glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

                DEBUG_FAILURE_MESSAGE(infoLog);
            }
        }
        
        for (const std::shared_ptr<Quanta::ShaderModule>& shader : shaderModules)
        {
            const ShaderModule* glShader = nullptr;

            if constexpr (DEBUG)
            {
                glShader = dynamic_cast<const ShaderModule*>(shader.get());

                DEBUG_ASSERT(glShader != nullptr);
            }
            else
            {
                glShader = static_cast<const ShaderModule*>(shader.get());
            }
            
            glDetachShader(handle, glShader->GetHandle());
        }
    }
    
    RasterPipeline::~RasterPipeline()
    {
        glDeleteProgram(handle);
    }

    const std::shared_ptr<Quanta::ShaderModule>& RasterPipeline::GetShaderModule(const USize index) const
    {
        DEBUG_ASSERT(index < shaderModules.size());

        return shaderModules[index];
    }
    
    const std::shared_ptr<Quanta::GraphicsBuffer>& RasterPipeline::GetUniformBuffer(const USize index) const
    {
        DEBUG_ASSERT(index < uniformBuffers.size());

        return uniformBuffers[index];
    }
    
    const std::shared_ptr<Quanta::GraphicsBuffer>& RasterPipeline::GetStorageBuffer(const USize index) const
    {
        DEBUG_ASSERT(index < storageBuffers.size());

        return storageBuffers[index];
    }

    const glm::uvec4& RasterPipeline::GetViewport() const
    {
        return viewport;
    }

    void RasterPipeline::SetViewport(const glm::uvec4& value)
    {
        viewport = value;

        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    }

    const glm::uvec4& RasterPipeline::GetScissorViewport() const
    {
        return scissorViewport;
    }
    
    void RasterPipeline::SetScissorViewport(const glm::uvec4& value)
    {
        scissorViewport = value;
    
        glScissor(value.x, value.y, value.z, value.w);
    } 

    USize RasterPipeline::GetShaderModuleCount() const
    {
        return shaderModules.size();
    }
    
    USize RasterPipeline::GetUniformBufferCount() const
    {
        return uniformBuffers.size();
    }
    
    USize RasterPipeline::GetStorageBufferCount() const
    {
        return storageBuffers.size();
    }

    PolygonFillMode RasterPipeline::GetPolygonFillMode() const
    {
        return polygonFillMode;
    }

    void RasterPipeline::SetPolygonFillMode(const PolygonFillMode value)
    {
        polygonFillMode = value;
    }

    DepthTestMode RasterPipeline::GetDepthTestMode() const
    {
        return depthTestMode;
    }

    void RasterPipeline::SetDepthTestMode(const DepthTestMode value)
    {
        depthTestMode = value;
    }

    bool RasterPipeline::GetEnableDepthWriting() const
    {
        return enableDepthWriting;
    }

    void RasterPipeline::SetEnableDepthWriting(const bool value)
    {
        enableDepthWriting = value;
    }

    bool RasterPipeline::GetEnableScissorTesting() const
    {
        return enableScissorTesting;
    }

    void RasterPipeline::SetEnableScissorTesting(const bool value)
    {
        enableScissorTesting = value;
    }

    BlendMode RasterPipeline::GetBlendMode() const
    {
        return blendMode;
    }

    void RasterPipeline::SetBlendMode(const BlendMode value)
    {
        blendMode = value;
    }

    BlendFactor RasterPipeline::GetBlendFactor() const
    {
        return blendFactor;
    }
    
    void RasterPipeline::SetBlendFactor(const BlendFactor value)
    {
        blendFactor = value;
    }

    FaceCullMode RasterPipeline::GetFaceCullMode() const
    {
        return faceCullMode;
    }

    void RasterPipeline::SetFaceCullMode(const FaceCullMode value)
    {
        faceCullMode = value;
    }

    GeometryLayout RasterPipeline::GetGeometryLayout() const
    {
        return geometryLayout;
    }

    void RasterPipeline::SetGeometryLayout(const GeometryLayout value)
    {
        geometryLayout = value;
    }

    GeometryWinding RasterPipeline::GetGeometryWinding() const
    {
        return geometryWinding;
    }

    void RasterPipeline::SetGeometryWinding(const GeometryWinding value)
    {
        geometryWinding = value;
    }
    
    U32 RasterPipeline::GetHandle() const
    {
        return handle;
    }
}
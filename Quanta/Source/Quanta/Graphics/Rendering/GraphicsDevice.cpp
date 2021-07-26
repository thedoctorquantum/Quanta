#include "GraphicsDevice.h"

#include <glad/glad.h>

namespace Quanta
{
    static VertexArray* vertexArray;
    static RasterPipeline* rasterPipeline;

    static GLenum geometryLayout;
    static GLenum indexType;
    
    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
    
    void GraphicsDevice::SetViewport(const glm::ivec4& viewport)
    {
        glViewportIndexedf(0, viewport.x, viewport.y, viewport.z, viewport.w);
    }

    void GraphicsDevice::SetScissorViewport(const glm::ivec4& viewport)
    {
        glScissorIndexed(0, viewport.x, viewport.y, viewport.z, viewport.w);
    }
    
    void GraphicsDevice::SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value)
    {
        if(!value)
        {
            rasterPipeline = nullptr;

            glUseProgram(0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDisable(GL_DEPTH_TEST);
            glDepthMask(false);
            
            glDisable(GL_SCISSOR_TEST);
            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glFrontFace(GL_CCW);

            return;
        }

        if(rasterPipeline != value.get())
        {
            for(size_t i = 0; i < value->GetUniformBuffers().size(); i++)
            {
                glBindBufferBase(GL_UNIFORM_BUFFER, i, value->GetUniformBuffers()[i]->GetHandle());
            }

            glUseProgram(value->GetHandle());
        }
        
        switch(value->GetPolygonFillMode())
        {
        case PolygonFillMode::Solid:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            break;
        case PolygonFillMode::Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            break; 

        case PolygonFillMode::Point:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

            break;
        }

        if((uint8_t) value->GetDepthTestMode())
        {
            glEnable(GL_DEPTH_TEST);

            switch(value->GetDepthTestMode())
            {
            case DepthTestMode::Always:
                glDepthFunc(GL_ALWAYS);
                
                break;
            case DepthTestMode::Never:
                glDepthFunc(GL_NEVER);

                break;
            case DepthTestMode::Equal:
                glDepthFunc(GL_EQUAL);

                break;
            case DepthTestMode::NotEqual:
                glDepthFunc(GL_NOTEQUAL);

                break;
            case DepthTestMode::Greater:
                glDepthFunc(GL_GREATER);

                break;
            case DepthTestMode::GreaterOrEqual:
                glDepthFunc(GL_GEQUAL);

                break;
            case DepthTestMode::Less:
                glDepthFunc(GL_LESS);

                break;
            case DepthTestMode::LessOrEqual:
                glDepthFunc(GL_LEQUAL);

                break;   
            }
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        glDepthMask(value->GetEnableDepthWriting());

        if(value->GetEnableScissorTesting())
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }

        if((uint8_t) value->GetBlendMode())
        {
            glEnable(GL_BLEND);

            switch(value->GetBlendMode())
            {
            case BlendMode::Add:
                glBlendEquation(GL_FUNC_ADD);

                break;
            case BlendMode::Subtract:
                glBlendEquation(GL_FUNC_SUBTRACT);

                break;
            case BlendMode::ReverseSubtract:
                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);

                break;
            }
        }
        else
        {
            glDisable(GL_BLEND);
        }
        
        switch(value->GetBlendFactor())
        {
        case BlendFactor::Zero:
            glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

            break;
        case BlendFactor::One:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            
            break;
        case BlendFactor::SourceAlpha:
            glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

            break;
        case BlendFactor::InverseSourceAlpha:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            break;
        case BlendFactor::SourceColor:
            glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

            break;
        case BlendFactor::InverseSourceColor:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

            break;
        }

        if((uint8_t) value->GetFaceCullMode())
        {
            glEnable(GL_CULL_FACE);

            switch(value->GetFaceCullMode())
            {
            case FaceCullMode::Back:
                glCullFace(GL_BACK);

                break;
            case FaceCullMode::Front:
                glCullFace(GL_FRONT);
                
                break;
            }
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        
        switch(value->GetGeometryLayout())
        {
        case GeometryLayout::Triangle:
            geometryLayout = GL_TRIANGLES;

            break;
        case GeometryLayout::TriangleStrip:
            geometryLayout = GL_TRIANGLE_STRIP;

            break;
        case GeometryLayout::Line:
            geometryLayout = GL_LINES;

            break;
        case GeometryLayout::Point:
            geometryLayout = GL_POINTS;
            
            break;
        }

        switch(value->GetGeometryWinding())
        {
        case GeometryWinding::CounterClockwise:
            glFrontFace(GL_CCW);

            break;
        case GeometryWinding::Clockise:
            glFrontFace(GL_CW);

            break;
        }

        rasterPipeline = value.get();
    }

    void GraphicsDevice::SetVertexArray(const std::shared_ptr<VertexArray>& value)
    {
        if(vertexArray == value.get()) return;

        if(!value)
        {
            vertexArray = nullptr;

            glBindVertexArray(0);

            return;
        }

        switch(value->GetIndexType())
        {
        case IndexType::UInt8:
            indexType = GL_UNSIGNED_BYTE;

            break;  
        case IndexType::UInt16: 
            indexType = GL_UNSIGNED_SHORT;

            break;
        case IndexType::UInt32:
            indexType = GL_UNSIGNED_INT;

            break;
        }
        
        glBindVertexArray(value->GetHandle());

        vertexArray = value.get();
    }
    
    void GraphicsDevice::BindTexture(uint32_t handle, uint32_t index)
    {
        glBindTextureUnit(index, handle);
    }
    
    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        glDrawElementsInstancedBaseVertexBaseInstance(
            geometryLayout, 
            command.Count, 
            indexType, 
            (void*) command.IndexOffset,
            command.InstanceCount, 
            command.StartVertex,
            command.StartInstance);
    }
}
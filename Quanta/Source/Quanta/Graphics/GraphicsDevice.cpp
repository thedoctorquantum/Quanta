#include "GraphicsDevice.h"

#include <glad/glad.h>

namespace Quanta
{
    static std::shared_ptr<VertexArray> currentVertexArray;
    static std::shared_ptr<RasterPipeline> currentRasterPipeline;

    static GLenum geometryType;
    static GLenum indexType;

    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
    
    void GraphicsDevice::Viewport(const glm::ivec4& viewport)
    {
        glViewportIndexedf(0, viewport.x, viewport.y, viewport.z, viewport.w);
    }
    
    void GraphicsDevice::SetPipeline(const std::shared_ptr<RasterPipeline>& pipeline)
    {
        if(currentRasterPipeline == pipeline) return;

        currentRasterPipeline = pipeline;
        
        if(currentRasterPipeline != nullptr)
        {
            glUseProgram(pipeline->GetHandle());
            
            switch(pipeline->GetFaceCullMode())
            {
            case FaceCullMode::None:
                glDisable(GL_CULL_FACE);

                break;
            case FaceCullMode::Back:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                break;
            case FaceCullMode::Front:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
            }
            
            switch(pipeline->GetGeometryLayout())
            {
            case GeometryLayout::Triangle:
                geometryType = GL_TRIANGLES;

                break;
            case GeometryLayout::TriangleStrip:
                geometryType = GL_TRIANGLE_STRIP;

                break;
            case GeometryLayout::Line:
                geometryType = GL_LINES;

                break;
            case GeometryLayout::Point:
                geometryType = GL_POINTS;
                
                break;
            }

            switch(pipeline->GetGeometryWinding())
            {
            case GeometryWinding::CounterClockwise:
                glFrontFace(GL_CCW);

                break;
            case GeometryWinding::Clockise:
                glFrontFace(GL_CW);

                break;
            }
        }
        else
        {
            glUseProgram(0);
        }
    }

    void GraphicsDevice::SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
    {
        if(currentVertexArray == vertexArray) return;

        currentVertexArray = vertexArray;
        
        if(vertexArray != nullptr)
        {
            glBindVertexArray(vertexArray->GetHandle());
        }
        else
        {
            glBindVertexArray(0);
        }
    }
    
    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        glDrawElementsInstancedBaseVertexBaseInstance(
            geometryType, 
            command.Count, 
            GL_UNSIGNED_INT, 
            nullptr,
            command.InstanceCount, 
            command.StartVertex,
            command.StartInstance);
    }
}
#include "GraphicsDevice.h"

#include <glad/glad.h>

namespace Quanta
{
    static std::shared_ptr<VertexArray> currentVertexArray;
    static std::shared_ptr<ShaderProgram> currentShader;

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
    
    void GraphicsDevice::SetPipeline(const std::shared_ptr<ShaderProgram>& shader)
    {
        if(currentShader == shader) return;

        currentShader = shader;
        
        if(shader != nullptr)
        {
            glUseProgram(shader->GetHandle());
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
            GL_TRIANGLES, 
            command.Count, 
            GL_UNSIGNED_INT, 
            nullptr,
            command.InstanceCount, 
            command.StartVertex,
            command.StartInstance);
    }
}
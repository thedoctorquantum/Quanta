#include "GraphicsDevice.h"

#include <glad/glad.h>

namespace Quanta
{
    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, &color.x);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
    
    void GraphicsDevice::Viewport(const glm::ivec4& viewport)
    {
        glViewportIndexedfv(0, (float*) &viewport.x);
    }
    
    void GraphicsDevice::SetVertexBuffer(const GraphicsBuffer& buffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer.GetHandle());
    }
}
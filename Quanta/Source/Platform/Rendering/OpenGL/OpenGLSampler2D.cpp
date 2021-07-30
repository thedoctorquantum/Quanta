#include <glad/glad.h>

#include "OpenGLSampler2D.h"

namespace Quanta
{
    OpenGLSampler2D::OpenGLSampler2D(const std::shared_ptr<Texture2D>& texture)
    {
        this->texture = texture;

        glCreateSamplers(1, &handle);
    }

    OpenGLSampler2D::~OpenGLSampler2D()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Texture2D>& OpenGLSampler2D::GetTexture() const
    {
        return texture;
    } 

    uint32_t OpenGLSampler2D::GetHandle() const
    {
        return handle;
    }
}
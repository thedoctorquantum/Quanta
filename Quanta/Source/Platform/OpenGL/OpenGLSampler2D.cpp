#include <glad/glad.h>

#include "OpenGLSampler2D.h"
#include "GLEnumConversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLSampler2D::OpenGLSampler2D(const std::shared_ptr<Texture2D>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);
        
        this->texture = texture;

        glCreateSamplers(1, &handle);

        DEBUG_ASSERT(handle != 0);
        
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnifiaction));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
    }
    
    OpenGLSampler2D::~OpenGLSampler2D()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Texture2D>& OpenGLSampler2D::GetTexture() const
    {
        return texture;
    } 

    FilterMode OpenGLSampler2D::GetMagnification() const
    {
        return magnifiaction;
    }

    void OpenGLSampler2D::SetMagnification(FilterMode value)
    {
        if(magnifiaction == value) return;

        magnifiaction = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }

    FilterMode OpenGLSampler2D::GetMinification() const
    {
        return minification;
    }

    void OpenGLSampler2D::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }

    WrapMode OpenGLSampler2D::GetWrapModeX() const
    {
        return wrapModeX;
    }

    void OpenGLSampler2D::SetWrapModeX(WrapMode value)
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler2D::GetWrapModeY() const
    {
        return wrapModeY;
    }

    void OpenGLSampler2D::SetWrapModeY(WrapMode value)
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }

    uint32_t OpenGLSampler2D::GetHandle() const
    {
        return handle;
    }
}
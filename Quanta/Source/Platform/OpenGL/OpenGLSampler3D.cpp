#include <glad/glad.h>

#include "OpenGLSampler3D.h"
#include "GLEnumConversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLSampler3D::OpenGLSampler3D(const std::shared_ptr<Texture3D>& texture)
    {
        DEBUG_ASSERT(texture != nullptr);

        this->texture = texture;

        glCreateSamplers(1, &handle);

        DEBUG_ASSERT(handle != 0);
        
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnification));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, WrapModeToGLenum(wrapModeZ));
    }
    
    OpenGLSampler3D::~OpenGLSampler3D()
    {
        glDeleteSamplers(1, &handle);
    }

    const std::shared_ptr<Texture3D>& OpenGLSampler3D::GetTexture() const
    {
        return texture;
    } 

    FilterMode OpenGLSampler3D::GetMagnification() const
    {
        return magnification;
    }

    void OpenGLSampler3D::SetMagnification(FilterMode value)
    {
        if(magnification == value) return;

        magnification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(value));
    }

    FilterMode OpenGLSampler3D::GetMinification() const
    {
        return minification;
    }

    void OpenGLSampler3D::SetMinification(FilterMode value)
    {
        if(minification == value) return;

        minification = value;

        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(value));
    }

    WrapMode OpenGLSampler3D::GetWrapModeX() const
    {
        return wrapModeX;
    }

    void OpenGLSampler3D::SetWrapModeX(WrapMode value)
    {
        if(wrapModeX == value) return;

        wrapModeX = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler3D::GetWrapModeY() const
    {
        return wrapModeY;
    }
    
    void OpenGLSampler3D::SetWrapModeY(WrapMode value)
    {
        if(wrapModeY == value) return;

        wrapModeY = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(value));
    }

    WrapMode OpenGLSampler3D::GetWrapModeZ() const
    {
        return wrapModeZ;
    }
    
    void OpenGLSampler3D::SetWrapModeZ(WrapMode value)
    {
        if(wrapModeZ == value) return;

        wrapModeZ = value;

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, WrapModeToGLenum(value));
    }

    uint32_t OpenGLSampler3D::GetHandle() const
    {
        return handle;
    }
}
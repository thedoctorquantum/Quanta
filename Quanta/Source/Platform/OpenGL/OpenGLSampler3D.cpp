#include <glad/glad.h>

#include "OpenGLSampler3D.h"

namespace Quanta
{
    static GLenum FilterModeToGLenum(FilterMode mode)
    {
        switch(mode)
        {
        case FilterMode::Linear: return GL_LINEAR;
        case FilterMode::Nearest: return GL_NEAREST;
        case FilterMode::NearestMipMapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case FilterMode::LinearMipMapLinear: return GL_LINEAR_MIPMAP_LINEAR;
        case FilterMode::NearestMipMapLinear: return GL_NEAREST_MIPMAP_LINEAR;
        case FilterMode::LinearMipMapNearest: return GL_LINEAR_MIPMAP_NEAREST;
        }

        return 0;
    }
    
    static GLenum WrapModeToGLenum(WrapMode mode)
    {
        switch(mode)
        {
        case WrapMode::Repeat: return GL_REPEAT;
        case WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        }

        return 0;
    }

    OpenGLSampler3D::OpenGLSampler3D(const std::shared_ptr<Texture3D>& texture)
    {
        this->texture = texture;

        glCreateSamplers(1, &handle);
        
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, FilterModeToGLenum(magnifiaction));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, FilterModeToGLenum(minification));

        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, WrapModeToGLenum(wrapModeX));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, WrapModeToGLenum(wrapModeY));
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
        return magnifiaction;
    }

    void OpenGLSampler3D::SetMagnification(FilterMode value)
    {
        if(magnifiaction == value) return;

        magnifiaction = value;

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
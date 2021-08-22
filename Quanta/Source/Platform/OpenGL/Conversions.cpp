#include "Conversions.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    GLenum FilterModeToGLenum(FilterMode mode)
    {
        DEBUG_ASSERT(
            mode == FilterMode::Linear ||
            mode == FilterMode::Nearest ||
            mode == FilterMode::NearestMipMapNearest ||
            mode == FilterMode::LinearMipMapLinear ||
            mode == FilterMode::NearestMipMapLinear ||
            mode == FilterMode::LinearMipMapNearest
        );

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
    
    GLenum WrapModeToGLenum(WrapMode mode)
    {
        DEBUG_ASSERT(
            mode == WrapMode::Repeat ||
            mode == WrapMode::MirroredRepeat ||
            mode == WrapMode::ClampToEdge ||
            mode == WrapMode::ClampToBorder
        );

        switch(mode)
        {
        case WrapMode::Repeat: return GL_REPEAT;
        case WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        }

        return 0;
    }

    GLenum TexelFormatToPixelFormat(TexelFormat format)
    {
        switch(format)
        {
        case TexelFormat::Rgba8I: return GL_RGBA;    
        case TexelFormat::Rgba16F: return GL_RGBA;
        case TexelFormat::Rgba32F: return GL_RGBA;                
        }

        return 0;
    }

    GLenum TexelFormatToInternalFormat(TexelFormat format)
    {
        switch(format)
        {
            case TexelFormat::Rgba8I: return GL_RGBA8;
            case TexelFormat::Rgba16F: return GL_RGBA16F;
            case TexelFormat::Rgba32F: return GL_RGBA32F;
        }

        return 0;
    }
    
    GLenum TexelFormatToPixelType(TexelFormat format)
    {
        switch(format)
        {
            case TexelFormat::Rgba8I: return GL_UNSIGNED_BYTE;
            case TexelFormat::Rgba16F: return GL_FLOAT;
            case TexelFormat::Rgba32F: return GL_FLOAT;
        }

        return 0;
    }
}
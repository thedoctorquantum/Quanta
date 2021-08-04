#include "GLEnumConversions.h"
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
}
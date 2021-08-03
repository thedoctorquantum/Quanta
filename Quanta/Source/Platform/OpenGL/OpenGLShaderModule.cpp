#include <iostream>
#include <glad/glad.h>

#include "OpenGLShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGLShaderModule::OpenGLShaderModule(ShaderType type, const std::string& source)
    {
        DEBUG_ASSERT(source.length() != 0);
        DEBUG_ASSERT(source.c_str() != nullptr);
        
        GLenum target;
        
        switch(type)
        {
        case ShaderType::Vertex:
            target = GL_VERTEX_SHADER;
            
            break;
        case ShaderType::Pixel:
            target = GL_FRAGMENT_SHADER;

            break;
        case ShaderType::Geometry:
            target = GL_GEOMETRY_SHADER;

            break;
        case ShaderType::Compute:
            target = GL_COMPUTE_SHADER;

            break;
        default:
            DEBUG_FAILURE_MESSAGE("ShaderType out of range");
        }
        
        handle = glCreateShader(target);
        
        DEBUG_ASSERT(handle != 0);

        const char* sourcePointer = source.c_str();
        int32_t length = source.length();
        
        glShaderSource(handle, 1, &sourcePointer, &length);
        glCompileShader(handle);

#if DEBUG
        int success;

        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            DEBUG_FAILURE_MESSAGE(infoLog);
        }
#endif
    };

    OpenGLShaderModule::~OpenGLShaderModule()
    {
        glDeleteShader(handle);
    }
    
    uint32_t OpenGLShaderModule::GetHandle() const
    {
        return handle;
    }
}

#include <iostream>
#include <glad/glad.h>

#include "OpenGLShaderModule.h"

namespace Quanta
{
    OpenGLShaderModule::OpenGLShaderModule(ShaderType type, const std::string& source)
    {
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
        }
        
        handle = glCreateShader(target);

        const char* sourcePointer = source.c_str();
        int32_t length = source.length();
        
        glShaderSource(handle, 1, &sourcePointer, &length);
        glCompileShader(handle);

        int success;

        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            std::cout << "Shader Compilation Failed:\n" << infoLog << std::endl;
        }
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

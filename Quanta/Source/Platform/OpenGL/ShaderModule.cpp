#include <glad/glad.h>

#include "ShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGL::ShaderModule::ShaderModule(ShaderType type, const std::string& source)
    {
        DEBUG_ASSERT(source.length() != 0);
        DEBUG_ASSERT(source.c_str() != nullptr);
        
        DEBUG_ASSERT(
            type == ShaderType::Vertex ||
            type == ShaderType::Pixel ||
            type == ShaderType::Geometry ||
            type == ShaderType::Compute
        );

        GLenum target = 0;
        
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
        
        DEBUG_ASSERT(handle != 0);

        const char* sourcePointer = source.c_str();
        USize length = source.length();
        
        glShaderSource(handle, 1, &sourcePointer, reinterpret_cast<GLint*>(&length));
        
        glCompileShader(handle);
        
        if constexpr (DEBUG)
        {
            int success;

            glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                char infoLog[512];

                glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

                printf("%s\n", infoLog);

                DEBUG_ASSERT(success);
            }
        }
    };

    OpenGL::ShaderModule::~ShaderModule()
    {
        glDeleteShader(handle);
    }
    
    U32 OpenGL::ShaderModule::GetHandle() const
    {
        return handle;
    }
}

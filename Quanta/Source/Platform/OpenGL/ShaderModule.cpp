#include <Quanta/CLI/Log/Log.h>
#include <glad/glad.h>

#include "ShaderModule.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenGL::ShaderModule::ShaderModule(const ShaderType type, const std::string& source)
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

        const char* const sourcePointer = source.c_str();
        const USize length = source.length();
        
        glShaderSource(handle, 1, &sourcePointer, reinterpret_cast<const GLint*>(&length));
        
        glCompileShader(handle);
        
        if constexpr (DEBUG)
        {
            int success = 0;

            glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                char infoLog[512];

                glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

                Log::Write(Log::Level::Error, infoLog);

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

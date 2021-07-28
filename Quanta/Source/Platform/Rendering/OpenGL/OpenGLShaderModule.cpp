#include <iostream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>

#include "OpenGLShaderModule.h"

namespace Quanta
{
    OpenGLShaderModule::OpenGLShaderModule(ShaderType type, const std::string& source)
    {
        GLenum target;
        shaderc_shader_kind shaderStage;
        
        switch(type)
        {
        case ShaderType::Vertex:
            target = GL_VERTEX_SHADER;
            shaderStage = shaderc_vertex_shader;
            
            break;
        case ShaderType::Fragment:
            target = GL_FRAGMENT_SHADER;
            shaderStage = shaderc_fragment_shader;

            break;
        case ShaderType::Geometry:
            target = GL_GEOMETRY_SHADER;
            shaderStage = shaderc_geometry_shader;

            break;
        case ShaderType::Compute:
            target = GL_COMPUTE_SHADER;
            shaderStage = shaderc_compute_shader;

            break;
        }

        handle = glCreateShader(target);

        const char* sourcePointer = source.c_str();
        int32_t length = source.length();

        //shaderc::Compiler compiler;
        //shaderc::CompileOptions options;

        //options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

        //shaderc::SpvCompilationResult compileResult = compiler.CompileGlslToSpv(source.c_str(), shaderStage, nullptr);

        //if(compileResult.GetCompilationStatus() != shaderc_compilation_status_success)
        //{
        //    std::cout << "Shader Compilation Failed:\n" << compileResult.GetErrorMessage() << std::endl;
        //}

        //std::vector<uint8_t> binary = std::vector<uint8_t>(compileResult.cbegin(), compileResult.cend());

        //glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size());
        //glSpecializeShader(handle, "main", 0, nullptr, nullptr);

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

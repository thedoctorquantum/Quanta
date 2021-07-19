#include "ShaderProgram.h"

#include <iostream>
#include <glad/glad.h>

namespace Quanta
{
    ShaderProgram::ShaderProgram(const std::vector<std::shared_ptr<ShaderModule>>& modules)
    {
        handle = glCreateProgram();

        for(int i = 0; i < modules.size(); i++)
        {
            glAttachShader(handle, modules[i]->GetHandle());
        }

        glLinkProgram(handle);

        int success;

        glGetProgramiv(handle, GL_LINK_STATUS, &success);

        if(!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

            std::cout << "Shader program " << handle << " could not link: \n" << infoLog << std::endl; 
        }

        for(int i = 0; i < modules.size(); i++)
        {
            glDetachShader(handle, modules[i]->GetHandle());
        }
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(handle);
    }
    
    uint32_t ShaderProgram::GetHandle() const
    {
        return handle;
    }
}
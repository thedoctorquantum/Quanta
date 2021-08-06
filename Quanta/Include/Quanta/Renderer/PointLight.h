#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    struct PointLight
    {
        glm::vec3 Position = glm::vec3(0.0f);
        
        float Padding0 = 0;

        glm::vec4 Ambient = glm::vec4(1.0f);
        glm::vec4 Diffuse = glm::vec4(0.0f);
        glm::vec4 Specular = glm::vec4(0.0f); 

        float Constant = 1.0f;
        float Linear = 0.0f;
        float Quadratic = 0.0f;
        float Padding1;
    };
}
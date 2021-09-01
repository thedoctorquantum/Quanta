#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    class PointLightComponent final
    {
    public:
        glm::vec3 ambient = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(0.0f);
        glm::vec3 specular = glm::vec3(0.0f); 

        float linear = 0.0f;
        float quadratic = 0.0f;
    };
}
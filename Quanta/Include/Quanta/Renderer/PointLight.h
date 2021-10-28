#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    struct PointLight
    {
        glm::vec3 Position = glm::vec3(0.0f);
        float Intensity = 1.0f; 
    public:
        glm::vec3 Ambient = glm::vec3(1.0f);
    private:
        float padding2 = 0.0f;
    public:
        glm::vec3 Diffuse = glm::vec3(0.0f);
    private: 
        float padding3 = 0.0f;
    public:
        glm::vec3 Specular = glm::vec3(0.0f); 

        float Linear = 0.0f;
        float Quadratic = 0.0f;
    private:
        float padding4 = 0.0f;
        float padding5 = 0.0f;
        float padding6 = 0.0f;
    };
}
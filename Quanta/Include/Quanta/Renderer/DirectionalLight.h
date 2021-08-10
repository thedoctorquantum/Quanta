#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    struct DirectionalLight final
    {
        glm::vec3 Direction = glm::vec3(0.0f);
    private:
        float padding1 = 0.0f;
    public:
        glm::vec3 Ambient = glm::vec3(1.0f);
    private:
        float padding2 = 0.0f;
    public:
        glm::vec3 Diffuse = glm::vec4(1.0f);
    private:
        float padding3 = 0.0f;
    public:
        glm::vec3 Specular = glm::vec4(1.0f);
    private: 
        float padding4 = 0.0f;
    };
}
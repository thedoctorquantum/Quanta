#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    struct Vertex
    {
        glm::vec3 Translation = glm::vec3(0.0f);
        glm::vec3 Normal = glm::vec3(0.0f);
        glm::vec2 Uv = glm::vec2(0.0f);
        glm::vec3 Tangent = glm::vec3(0.0f);
        glm::vec3 Color = glm::vec3(1.0f);
    };
}
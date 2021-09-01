#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Quanta
{
    class TransformComponent final
    {
    public:
        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        static TransformComponent Translate(const glm::vec3& translation);
        static TransformComponent Rotate(const glm::vec3& rotation);
        static TransformComponent Scale(const glm::vec3& scale);

        glm::mat4 CreateMatrix() const;
    };
}
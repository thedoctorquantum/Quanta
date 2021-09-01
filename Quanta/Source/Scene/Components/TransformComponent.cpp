#include <Quanta/Scene/Components/TransformComponent.h>

namespace Quanta
{
    TransformComponent Translate(const glm::vec3& translation)
    {   
        TransformComponent value;

        value.translation = translation;

        return value;
    }

    TransformComponent Rotate(const glm::vec3& rotation)
    {
        TransformComponent value;

        value.rotation = rotation;

        return value;
    }   

    TransformComponent Scale(const glm::vec3& scale)
    {
        TransformComponent value;

        value.scale = scale;

        return value;
    }   
    
    glm::mat4 TransformComponent::CreateMatrix() const
    {
        glm::mat4 matrix(1.0f);

        matrix = glm::scale(matrix, scale);
        matrix = glm::translate(matrix, translation);
        matrix *= glm::toMat4(glm::quat(glm::radians(rotation)));

        return matrix;
    }
}
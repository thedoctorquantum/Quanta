#pragma once

#include <glm/glm.hpp>
#include "../Graphics/Texture/Sampler.h"

namespace Quanta
{
    struct Material
    {
        glm::vec3 albedo = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(0.0f);

        float shininess = 1.0f;
        float opacity = 1.0f;
        
        std::shared_ptr<Sampler> albedoSampler = nullptr;
        std::shared_ptr<Sampler> specularSampler = nullptr;
        std::shared_ptr<Sampler> normalSampler = nullptr;
        std::shared_ptr<Sampler> opacitySampler = nullptr;

        Material() = default;

        Material(const Material&) = delete;

        Material& operator=(const Material&) = delete;

        Material(Material&&);

        Material& operator=(Material&&);
    };
}
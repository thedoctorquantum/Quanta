#pragma once

#include <glm/glm.hpp>
#include "../Graphics/Texture/Sampler.h"

namespace Quanta
{
    class Material final
    {
    public:
        Material();
        ~Material();

        Material(const Material&) = delete;

        Material& operator=(const Material&) = delete;

        Material(Material&&);

        Material& operator=(Material&&);

        const glm::vec3& GetAlbedo() const;
        void SetAlbedo(const glm::vec3&);
        
        const glm::vec3& GetDiffuse() const;
        void SetDiffuse(const glm::vec3&);

        const glm::vec3& GetSpecular() const;
        void SetSpecular(const glm::vec3&);
        
        float GetShininess() const;
        void SetShininess(float);

        float GetOpacity() const;
        void SetOpacity(float);
        
        std::shared_ptr<Sampler> GetAlbedoSampler() const;
        void SetAlbedoSampler(std::shared_ptr<Sampler>);

        std::shared_ptr<Sampler> GetSpecularSampler() const;
        void SetSpecularSampler(std::shared_ptr<Sampler>);
        
        std::shared_ptr<Sampler> GetNormalSampler() const;
        void SetNormalSampler(std::shared_ptr<Sampler>);

        std::shared_ptr<Sampler> GetOpacitySampler() const;
        void SetOpacitySampler(std::shared_ptr<Sampler>);
    private:    
        glm::vec3 albedo = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(0.0f);
        float shininess = 1.0f;
        float opacity = 1.0f;
        
        std::shared_ptr<Sampler> albedoSampler = nullptr;
        std::shared_ptr<Sampler> specularSampler = nullptr;
        std::shared_ptr<Sampler> normalSampler = nullptr;
        std::shared_ptr<Sampler> opacitySampler = nullptr;
    };
}
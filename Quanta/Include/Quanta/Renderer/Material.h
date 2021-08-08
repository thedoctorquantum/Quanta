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

        const glm::vec4& GetAlbedo() const;
        void SetAlbedo(const glm::vec4&);
        
        const glm::vec4& GetDiffuse() const;
        void SetDiffuse(const glm::vec4&);

        const glm::vec4& GetSpecular() const;
        void SetSpecular(const glm::vec4&);
        
        float GetShininess() const;
        void SetShininess(float);

        Sampler* GetAlbedoSampler() const;
        void SetAlbedoSampler(Sampler*);

        Sampler* GetDiffuseSampler() const;
        void SetDiffuseSampler(Sampler*);

        Sampler* GetSpecularSampler() const;
        void SetSpecularSampler(Sampler*);
        
        Sampler* GetNormalSampler() const;
        void SetNormalSampler(Sampler*);
    private:    
        glm::vec4 albedo = glm::vec4(1.0f);
        glm::vec4 diffuse = glm::vec4(1.0f);
        glm::vec4 specular = glm::vec4(0.0f);
        float shininess = 1.0f;
        
        Sampler* albedoSampler = nullptr;
        Sampler* diffuseSampler = nullptr;
        Sampler* specularSampler = nullptr;
        Sampler* normalSampler = nullptr;
    };
}
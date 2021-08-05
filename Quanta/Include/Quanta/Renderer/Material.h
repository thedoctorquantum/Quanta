#pragma once

#include <glm/glm.hpp>
#include "../Graphics/Texture/Sampler2D.h"

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

        Sampler2D* GetAlbedoSampler() const;
        void SetAlbedoSampler(Sampler2D*);

        Sampler2D* GetDiffuseSampler() const;
        void SetDiffuseSampler(Sampler2D*);

        Sampler2D* GetSpecularSampler() const;
        void SetSpecularSampler(Sampler2D*);
    private:    
        glm::vec4 albedo = glm::vec4(1.0f);
        glm::vec4 diffuse = glm::vec4(0.0f);
        glm::vec4 specular = glm::vec4(0.0f);
        float shininess = 0.0f;
        
        Sampler2D* albedoSampler = nullptr;
        Sampler2D* diffuseSampler = nullptr;
        Sampler2D* specularSampler = nullptr;
    };
}
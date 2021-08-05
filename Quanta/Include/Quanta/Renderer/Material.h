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

        const glm::vec4& GetColor() const;
        void SetColor(const glm::vec4&);

        const Sampler2D* GetAlbedoSampler() const;
        Sampler2D* GetAlbedoSampler();
        
        void SetAlbedoSampler(Sampler2D*);
    private:    
        Sampler2D* albedoSampler = nullptr;
        glm::vec4 color = glm::vec4(1.0f);
    };
}
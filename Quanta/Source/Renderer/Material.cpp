#include <Quanta/Renderer/Material.h>

namespace Quanta
{
    Material::Material(Material&& other)
    {
        *this = std::move(other);
    }
    
    Material& Material::operator=(Material&& other)
    {
        albedoSampler = other.albedoSampler;
        specularSampler = other.specularSampler;
        normalSampler = other.normalSampler;
        opacitySampler = other.opacitySampler;

        albedo = other.albedo;
        diffuse = other.diffuse;
        specular = other.specular;
        shininess = other.shininess;
        opacity = other.opacity;

        other.albedoSampler = nullptr;
        other.specularSampler = nullptr;
        other.normalSampler = nullptr; 
        other.opacitySampler = nullptr;

        other.albedo = { };
        other.diffuse = { };
        other.specular = { };

        return *this;
    }
}
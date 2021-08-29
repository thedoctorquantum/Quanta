#include <Quanta/Renderer/Material.h>

namespace Quanta
{
    Material::Material()
    {

    }

    Material::~Material()
    {

    }

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

    const glm::vec3& Material::GetAlbedo() const
    {
        return albedo;
    }

    void Material::SetAlbedo(const glm::vec3& value)
    {
        albedo = value;
    }

    const glm::vec3& Material::GetDiffuse() const
    {   
        return diffuse;
    }
    
    void Material::SetDiffuse(const glm::vec3& value)
    {
        diffuse = value;
    }

    const glm::vec3& Material::GetSpecular() const
    {
        return specular;
    }

    void Material::SetSpecular(const glm::vec3& value)
    {
        specular = value;
    }
    
    float Material::GetShininess() const
    {
        return shininess;
    }

    void Material::SetShininess(const float value)
    {
        shininess = value;
    }

    float Material::GetOpacity() const
    {
        return opacity;
    }        
    
    void Material::SetOpacity(const float value)
    {
        opacity = value;
    }

    std::shared_ptr<Sampler> Material::GetAlbedoSampler() const
    {
        return albedoSampler;
    }
    
    void Material::SetAlbedoSampler(const std::shared_ptr<Sampler>& value)
    {
        albedoSampler = value;
    }
        
    std::shared_ptr<Sampler> Material::GetSpecularSampler() const
    {
        return specularSampler;
    }

    void Material::SetSpecularSampler(const std::shared_ptr<Sampler>& value)
    {
        specularSampler = value;
    }

    std::shared_ptr<Sampler> Material::GetNormalSampler() const
    {
        return normalSampler;
    }
    
    void Material::SetNormalSampler(const std::shared_ptr<Sampler>& value)
    {
        normalSampler = value;
    }

    std::shared_ptr<Sampler> Material::GetOpacitySampler() const
    {
        return opacitySampler;
    }
    
    void Material::SetOpacitySampler(const std::shared_ptr<Sampler>& value)
    {
        opacitySampler = value;
    }
}
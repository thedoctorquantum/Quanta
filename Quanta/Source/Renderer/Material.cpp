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
        
    }

    Material& Material::operator=(Material&& other)
    {
        return *this;
    }

    const glm::vec4& Material::GetAlbedo() const
    {
        return albedo;
    }

    void Material::SetAlbedo(const glm::vec4& value)
    {
        albedo = value;
    }

    const glm::vec4& Material::GetDiffuse() const
    {   
        return diffuse;
    }
    
    void Material::SetDiffuse(const glm::vec4& value)
    {
        diffuse = value;
    }

    const glm::vec4& Material::GetSpecular() const
    {
        return specular;
    }

    void Material::SetSpecular(const glm::vec4& value)
    {
        specular = value;
    }
    
    float Material::GetShininess() const
    {
        return shininess;
    }

    void Material::SetShininess(float value)
    {
        shininess = value;
    }
    
    Sampler* Material::GetAlbedoSampler() const
    {
        return albedoSampler;
    }
    
    void Material::SetAlbedoSampler(Sampler* value)
    {
        albedoSampler = value;
    }

    Sampler* Material::GetDiffuseSampler() const
    {
        return diffuseSampler;
    }

    void Material::SetDiffuseSampler(Sampler* value)
    {
        diffuseSampler = value;
    }
        
    Sampler* Material::GetSpecularSampler() const
    {
        return specularSampler;
    }

    void Material::SetSpecularSampler(Sampler* value)
    {
        specularSampler = value;
    }

    Sampler* Material::GetNormalSampler() const
    {
        return normalSampler;
    }
    
    void Material::SetNormalSampler(Sampler* value)
    {
        normalSampler = value;
    }
}
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

    const glm::vec4& Material::GetColor() const
    {
        return color;
    }

    void Material::SetColor(const glm::vec4& value)
    {
        color = value;
    }

    const Sampler2D* Material::GetAlbedoSampler() const
    {
        return albedoSampler;
    }

    Sampler2D* Material::GetAlbedoSampler()
    {
        return albedoSampler;
    }

    void Material::SetAlbedoSampler(Sampler2D* value)
    {
        albedoSampler = value;
    }
}
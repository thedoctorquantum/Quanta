#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Mesh.h" 
#include "Material.h"

namespace Quanta
{
    struct Model
    {
        struct Part
        {
            Mesh mesh;
            glm::mat4 transform = glm::mat4(1.0f);

            size_t materialIndex = 0;
        };

        static Model FromFile(const std::string& filepath);

        Model() = default;

        Model(Model&&);
        
        Model& operator=(Model&&);

        Model(const Model&) = delete;

        Model& operator=(const Model&) = delete;        

        std::vector<Part> parts;
        std::vector<Material> materials;
    };
}
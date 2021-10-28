#include <Quanta/Renderer/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Debugging/Validation.h"

namespace Quanta
{
    static std::string GetDirectory(const std::string& filepath)
    {
        std::size_t pos = filepath.find_last_of("/");

        return std::string::npos == pos ? "" : filepath.substr(0, pos);
    }

    Model Model::FromFile(const std::string& filepath)
    {
        Assimp::Importer importer;

        Model model;
        
        const auto scene = importer.ReadFile(filepath, 
            aiProcess_Triangulate | 
            aiProcess_CalcTangentSpace |
            aiProcess_FlipUVs |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_PreTransformVertices |
            aiProcess_GlobalScale |
            aiProcess_OptimizeMeshes
        ); 
        
        DEBUG_ASSERT(scene != nullptr);

        for (std::size_t i = 0; i < scene->mNumMeshes; i++)
        {
            const auto meshData = scene->mMeshes[i];

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            Mesh mesh;

            for (size_t j = 0; j < meshData->mNumVertices; j++)
            {                
                Vertex vertex;

                const auto& translation = meshData->mVertices[j];
                const auto& normal = meshData->mNormals[j];

                aiVector3D uv(0.0f);

                if (meshData->HasTextureCoords(0))
                {
                    uv = meshData->mTextureCoords[0][j]; 
                }

                aiColor4D color(1.0f);

                if (meshData->HasVertexColors(0))
                {
                    color = meshData->mColors[0][j]; 
                }

                const auto& tangent = meshData->mTangents[j];
                
                vertex.Translation = glm::vec3(translation.x, translation.y, translation.z);
                vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
                vertex.Uv = glm::vec2(uv.x, uv.y);
                vertex.Color = glm::vec3(color.r, color.g, color.b);
                vertex.Tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

                vertices.push_back(vertex);
            }

            for (std::size_t j = 0; j < meshData->mNumFaces; j++)
            {
                const auto& face = meshData->mFaces[j];

                for (std::size_t k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(face.mIndices[k]);
                }
            }

            mesh.SetVertices(vertices.data(), vertices.size());
            mesh.SetIndices(indices.data(), indices.size());    

            model.parts.push_back({ std::move(mesh), glm::mat4(1.0f), meshData->mMaterialIndex });    
        }

        const std::string directory = GetDirectory(filepath);

        for (std::size_t i = 0; i < scene->mNumMaterials; i++)
        {
            const auto materialData = scene->mMaterials[i];

            Material material;

            aiColor3D color(1.0f);
            float shininess = 1.0f;
            float opacity = 1.0f;
            
            if (materialData->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
            {
                material.albedo = { color.r, color.g, color.b };
            }
            
            if (materialData->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
            {
                material.specular = { color.r, color.g, color.b };
            }

            if (materialData->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
            {
                material.shininess = shininess;
            }

            if (materialData->Get(AI_MATKEY_OPACITY, opacity) == aiReturn_SUCCESS)
            {
                material.opacity = opacity;
            }

            aiString path;
            
            if (materialData->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
            {                
                const auto fullPath = directory + '/' + path.data;
                
                const auto albedo = Texture::Load2D(fullPath);
                
                material.albedoSampler = Sampler::Create(albedo);   
            }
            
            if (materialData->GetTexture(aiTextureType_SPECULAR, 0, &path) == aiReturn_SUCCESS)
            {
                const auto fullPath = directory + '/' + path.data;

                const auto specular = Texture::Load2D(fullPath);
                
                material.specularSampler = Sampler::Create(specular);
            }

            if (materialData->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
            {
                const auto fullPath = directory + '/' + path.data;

                const auto normal = Texture::Load2D(fullPath);
                
                material.normalSampler = Sampler::Create(normal);
            }

            if (materialData->GetTexture(aiTextureType_OPACITY, 0, &path) == aiReturn_SUCCESS)
            {
                const auto fullPath = directory + '/' + path.data;

                const auto opacity = Texture::Load2D(fullPath);
                
                material.opacitySampler = Sampler::Create(opacity);
            }

            model.materials.push_back(std::move(material));
        }

        if (model.materials.empty())
        {
            model.materials.emplace_back();

            for (auto& part : model.parts)
            {
                part.materialIndex = 0;
            }
        }

        return model;
    }

    Model::Model(Model&& other)
    {
        *this = std::move(other);
    }
    
    Model& Model::operator=(Model&& other)
    {
        parts = std::move(other.parts);
        materials = std::move(other.materials);

        return *this;
    }
}
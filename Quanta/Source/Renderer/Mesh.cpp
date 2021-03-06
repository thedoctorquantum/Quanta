#include <Quanta/Renderer/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    Mesh Mesh::FromFile(const std::string& filepath)
    {
        Assimp::Importer importer;

        Mesh mesh;

        const auto scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace); 

        DEBUG_ASSERT(scene != nullptr);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        size_t indexOffset = 0;
        
        for(size_t i = 0; i < scene->mNumMeshes; i++)
        {
            const auto mesh = scene->mMeshes[i];

            for(size_t j = 0; j < mesh->mNumVertices; j++)
            {                
                Vertex vertex;

                const auto& translation = mesh->mVertices[j];
                const auto& normal = mesh->mNormals[j];

                aiVector3D uv(0.0f);

                if(mesh->HasTextureCoords(0))
                {
                    uv = mesh->mTextureCoords[0][j]; 
                }

                aiColor4D color(1.0f);

                if(mesh->HasVertexColors(0))
                {
                    color = mesh->mColors[0][j]; 
                }

                const auto& tangent = mesh->mTangents[j];
                
                vertex.Translation = glm::vec3(translation.x, translation.y, translation.z);
                vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
                vertex.Uv = glm::vec2(uv.x, uv.y);
                vertex.Color = glm::vec3(color.r, color.g, color.b);
                vertex.Tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

                vertices.push_back(vertex);
            }

            size_t indexCount = 0;

            for(size_t j = 0; j < mesh->mNumFaces; j++)
            {
                const auto& face = mesh->mFaces[j];

                for(size_t k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(indexOffset + face.mIndices[k]);
                    
                    indexCount++;
                }
            }

            indexOffset += indexCount;
        }

        mesh.SetVertices(vertices.data(), vertices.size());
        mesh.SetIndices(indices.data(), indices.size());    

        return mesh;
    }
    
    Mesh::Mesh() : Mesh(0, 0)
    {
        
    }
    
    Mesh::Mesh(const size_t vertexCount, const size_t indexCount)
    {
        this->vertexCount = vertexCount;
        this->indexCount = indexCount;

        vertexArray = VertexArray::Create();

        const auto vertexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, vertexCount * sizeof(Vertex));
        const auto indexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, indexCount * sizeof(uint32_t));

        VertexLayout layout;

        layout.Add({
            BufferPrimitive::Float,
            3,
            sizeof(float),
            false
        });
        
        layout.Add({
            BufferPrimitive::Float,
            3,
            sizeof(float),
            false
        });
        
        layout.Add({
            BufferPrimitive::Float,
            2,
            sizeof(float),
            false
        });

        layout.Add({
            BufferPrimitive::Float,
            3,
            sizeof(float),
            false
        });

        layout.Add({
            BufferPrimitive::Float,
            3,
            sizeof(float),
            false
        });

        vertexArray->SetVertexBuffer(vertexBuffer, layout);
        vertexArray->SetIndexBuffer(indexBuffer, IndexType::UInt32);
    }
    
    Mesh::~Mesh()
    {

    }
    
    Mesh::Mesh(Mesh&& other)
    {
        *this = std::move(other);
    }
    
    Mesh& Mesh::operator=(Mesh&& other)
    {
        vertexArray = other.vertexArray;
        vertexCount = other.vertexCount;
        indexCount = other.indexCount;

        other.vertexArray = nullptr;
        other.vertexCount = 0;
        other.indexCount = 0;

        return *this;
    }
    
    void Mesh::SetVertices(const Vertex* vertices, const size_t count)
    {
        DEBUG_ASSERT(vertices != nullptr);

        const auto size = count * sizeof(Vertex);

        if(count > vertexCount)
        {
            vertexArray->GetVertexBuffer()->Resize(size);
        }

        vertexArray->GetVertexBuffer()->SetData(vertices, size);

        this->vertexCount = count;
    }
    
    void Mesh::SetIndices(const uint32_t* indices, const size_t count)
    {
        DEBUG_ASSERT(indices != nullptr);

        const auto size = count * sizeof(uint32_t);
        
        if(count > indexCount)
        {
            vertexArray->GetIndexBuffer()->Resize(size);
        }

        vertexArray->GetIndexBuffer()->SetData(indices, size);

        this->indexCount = count;
    }

    const Vertex* Mesh::GetVertices() const
    {
        return nullptr;
    }
    
    Vertex* Mesh::GetVertices()
    {
        return nullptr;
    }
    
    const uint32_t* Mesh::GetIndices() const
    {
        return nullptr;
    }
    
    uint32_t* Mesh::GetIndices()
    {
        return nullptr;
    }

    size_t Mesh::GetVertexCount() const
    {
        return vertexCount;
    }

    size_t Mesh::GetIndexCount() const
    {
        return indexCount;
    }   

    const std::shared_ptr<VertexArray>& Mesh::GetVertexArray() const
    {
        return vertexArray;
    }
}
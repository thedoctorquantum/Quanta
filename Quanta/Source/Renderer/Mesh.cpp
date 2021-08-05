#include <Quanta/Renderer/Mesh.h>

#include "../Debugging/Validation.h"

namespace Quanta
{
    Mesh Mesh::FromFile(const std::string& filepath)
    {
        return { };
    }
    
    Mesh::Mesh() : Mesh(0, 0)
    {
        
    }
    
    Mesh::Mesh(size_t vertexCount, size_t indexCount)
    {
        this->vertexCount = vertexCount;
        this->indexCount = indexCount;

        vertexArray = VertexArray::Create();

        std::shared_ptr<GraphicsBuffer> vertexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, vertexCount * sizeof(Vertex));
        std::shared_ptr<GraphicsBuffer> indexBuffer = GraphicsBuffer::Create(BufferUsage::Dynamic, indexCount * sizeof(uint32_t));

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
            4,
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
        DEBUG_ASSERT(false);

        vertexArray = std::move(other.vertexArray);
        vertexCount = other.vertexCount;
        indexCount = other.indexCount;

        other.vertexCount = 0;
        other.indexCount = 0;
    }
    
    Mesh& Mesh::operator=(Mesh&& other)
    {
        DEBUG_ASSERT(false);

        vertexArray = std::move(other.vertexArray);
        vertexCount = other.vertexCount;
        indexCount = other.indexCount;

        other.vertexCount = 0;
        other.indexCount = 0;

        return *this;
    }
    
    void Mesh::SetVertices(const Vertex* vertices, size_t count)
    {
        DEBUG_ASSERT(vertices != nullptr);

        size_t size = count * sizeof(Vertex);
 
        if(count > vertexCount)
        {
            GraphicsBuffer::Resize(*vertexArray->GetVertexBuffer(), size);
        }

        vertexArray->GetVertexBuffer()->SetData(vertices, size);

        this->vertexCount = count;
    }

    void Mesh::SetIndices(const uint32_t* indices, size_t count)
    {
        DEBUG_ASSERT(indices != nullptr);

        size_t size = count * sizeof(uint32_t);

        if(count > vertexCount)
        {
            GraphicsBuffer::Resize(*vertexArray->GetIndexBuffer(), size);
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
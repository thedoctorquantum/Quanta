#pragma once

#include <string>

#include "../Graphics/Buffer/VertexArray.h"
#include "Vertex.h"

namespace Quanta
{
    struct Mesh
    {
        static Mesh FromFile(const std::string& filepath);
        
        Mesh();
        
        Mesh(size_t vertexCount, size_t indexCount);
        
        ~Mesh();

        Mesh(Mesh&&);
        
        Mesh& operator=(Mesh&&);

        Mesh(const Mesh&) = delete;

        Mesh& operator=(const Mesh&) = delete;
        
        void SetVertices(const Vertex* vertices, size_t count);
        void SetIndices(const uint32_t* indices, size_t count);

        const Vertex* GetVertices() const;
        Vertex* GetVertices();

        const uint32_t* GetIndices() const;
        uint32_t* GetIndices();

        size_t GetVertexCount() const;
        size_t GetIndexCount() const;

        const std::shared_ptr<VertexArray>& GetVertexArray() const;
    private:
        std::shared_ptr<VertexArray> vertexArray;

        size_t vertexCount = 0;
        size_t indexCount = 0;
    };
}
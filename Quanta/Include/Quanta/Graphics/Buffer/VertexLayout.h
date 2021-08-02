#pragma once

#include <vector>

#include "VertexElement.h"

namespace Quanta
{    
    struct VertexLayout final
    {
    public:
        const VertexElement& operator[](uint32_t index) const;

        void Add(const VertexElement& element);

        uint32_t GetCount() const;
        uint32_t GetStride() const;
    private:
        std::vector<VertexElement> elements;
        uint32_t stride = 0;
    };
}
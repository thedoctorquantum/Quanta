#pragma once

#include <vector>

#include "VertexElement.h"

namespace Quanta
{    
    struct VertexLayout final
    {
    public:
        const VertexElement& operator[](Size index) const;

        void Add(const VertexElement& element);

        Size GetCount() const;
        Size GetStride() const;
    private:
        std::vector<VertexElement> elements;
        Size stride = 0;
    };
}
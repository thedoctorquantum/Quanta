#pragma once

#include <vector>

#include "VertexElement.h"

namespace Quanta
{    
    struct VertexLayout final
    {
    public:
        const VertexElement& operator[](USize index) const;

        void Add(const VertexElement& element);

        USize GetCount() const;
        USize GetStride() const;
    private:
        std::vector<VertexElement> elements;
        USize stride = 0;
    };
}
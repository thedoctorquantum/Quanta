#include <Quanta/Graphics/Buffer/VertexLayout.h>

namespace Quanta
{
    const VertexElement& VertexLayout::operator[](uint32_t index) const
    {
        return elements[index];            
    }
    
    void VertexLayout::Add(const VertexElement& element) 
    {
        elements.push_back(element);

        stride += element.Count * element.Size;
    }

    uint32_t VertexLayout::GetCount() const
    {
        return elements.size();
    }

    uint32_t VertexLayout::GetStride() const
    {
        return stride;
    }
}
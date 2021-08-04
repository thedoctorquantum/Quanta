#include <Quanta/Graphics/Buffer/VertexLayout.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    const VertexElement& VertexLayout::operator[](uint32_t index) const
    {
        DEBUG_ASSERT(index < elements.size());
        
        return elements[index];            
    }
    
    void VertexLayout::Add(const VertexElement& element) 
    {
        DEBUG_ASSERT(element.Count != 0);
        DEBUG_ASSERT(element.Size != 0);
        
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
#include <Quanta/Graphics/Buffer/VertexLayout.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    const VertexElement& VertexLayout::operator[](Size index) const
    {
        DEBUG_ASSERT(index < elements.size());
        
        return elements[index];            
    }
    
    void VertexLayout::Add(const VertexElement& element) 
    {
        DEBUG_ASSERT(element.count != 0);
        DEBUG_ASSERT(element.size != 0);
        
        elements.push_back(element);

        stride += element.count * element.size;
    }

    Size VertexLayout::GetCount() const
    {
        return elements.size();
    }

    Size VertexLayout::GetStride() const
    {
        return stride;
    }
}
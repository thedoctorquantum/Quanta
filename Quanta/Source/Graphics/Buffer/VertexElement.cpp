#include <Quanta/Graphics/Buffer/VertexElement.h>

namespace Quanta
{
    VertexElement::VertexElement(const BufferPrimitive primitive, const U32 count, const USize size, const bool normalized)
    {
        this->primitve = primitive;
        this->count = count;
        this->size = size;
        this->normalized = normalized;
    }
}
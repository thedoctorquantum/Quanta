#include <Quanta/Graphics/Buffer/VertexElement.h>

namespace Quanta
{
    VertexElement::VertexElement(BufferPrimitive primitive, U32 count, USize size, bool normalized)
    {
        this->primitve = primitive;
        this->count = count;
        this->size = size;
        this->normalized = normalized;
    }
}
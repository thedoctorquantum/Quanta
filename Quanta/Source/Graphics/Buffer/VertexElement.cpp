#include <Quanta/Graphics/Buffer/VertexElement.h>

namespace Quanta
{
    VertexElement::VertexElement(BufferPrimitive primitive, UInt32 count, Size size, bool normalized)
    {
        this->primitve = primitive;
        this->count = count;
        this->size = size;
        this->normalized = normalized;
    }
}
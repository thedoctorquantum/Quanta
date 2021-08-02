#include <Quanta/Graphics/Rendering/Buffer/VertexElement.h>

namespace Quanta
{
    VertexElement::VertexElement(BufferPrimitive primitive, uint32_t count, size_t size, bool normalized)
    {
        Primitve = primitive;
        Count = count;
        Size = size;
        Normalized = normalized;
    }
}
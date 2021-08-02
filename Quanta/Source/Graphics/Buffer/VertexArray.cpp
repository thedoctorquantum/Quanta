#include <Quanta/Graphics/Buffer/VertexArray.h>

#include "../../Platform/OpenGL/OpenGLVertexArray.h"

namespace Quanta
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
    
    VertexArray::~VertexArray() = default;
}
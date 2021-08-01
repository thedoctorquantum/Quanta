#include "VertexArray.h"
#include "../../../../Platform/Rendering/OpenGL/OpenGLVertexArray.h"

namespace Quanta
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
    
    VertexArray::~VertexArray() = default;
}
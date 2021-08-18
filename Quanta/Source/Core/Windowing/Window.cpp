#include <Quanta/Core/Windowing/Window.h>

#include "../../Platform/Glfw/Window.h"

namespace Quanta
{
    std::shared_ptr<Window> Window::Create(GraphicsApi graphicsApi)
    {
        return std::make_shared<Glfw::Window>(graphicsApi);
    }
};
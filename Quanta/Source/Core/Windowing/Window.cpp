#include <Quanta/Core/Windowing/Window.h>

#include "../../Platform/Glfw/GlfwWindow.h"

namespace Quanta
{
    std::shared_ptr<Window> Window::Create(GraphicsApi graphicsApi)
    {
        return std::make_shared<GlfwWindow>(graphicsApi);
    }
    
    Window::~Window() = default;
};
#include <Quanta/Core/Windowing/Window.h>

#include "../../Platform/Glfw/GlfwWindow.h"

namespace Quanta
{
    std::shared_ptr<Window> Window::Create()
    {
        return std::make_unique<GlfwWindow>();
    }

    Window::~Window() = default;
};
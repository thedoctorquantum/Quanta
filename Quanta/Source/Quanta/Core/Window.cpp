#include <glad/glad.h>
#include "Window.h"

namespace Quanta
{
    Window::Window(const std::string& title, glm::ivec2 size)
    {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(handle);

        gladLoadGL();
    }

    Window::~Window()
    {
        glfwDestroyWindow(handle);
    }

    bool Window::Exists() const
    {
        return !glfwWindowShouldClose(handle);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(handle);
    }
};
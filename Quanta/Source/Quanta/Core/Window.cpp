#include <glad/glad.h>
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Quanta
{
    Window::Window(const std::string& title, glm::ivec2 size)
    {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent((GLFWwindow*) handle);

        gladLoadGL();
    }

    Window::~Window()
    {
        glfwDestroyWindow((GLFWwindow*) handle);
    }

    bool Window::Exists() const
    {
        return !glfwWindowShouldClose((GLFWwindow*) handle);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }
    
    void Window::SwapBuffers() const
    {
        glfwSwapBuffers((GLFWwindow*) handle);
    }
};
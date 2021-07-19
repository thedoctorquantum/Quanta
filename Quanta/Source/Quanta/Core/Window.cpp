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

    glm::ivec2 Window::GetPosition() const
    {
        glm::ivec2 position;

        glfwGetWindowPos((GLFWwindow*) handle, &position.x, &position.y);

        return position;
    }
    
    glm::ivec2 Window::GetSize() const
    {
        glm::ivec2 size;

        glfwGetWindowSize((GLFWwindow*) handle, &size.x, &size.y);

        return size;
    }

    glm::ivec4 Window::GetBounds() const
    {
        glm::ivec4 bounds;

        glfwGetWindowPos((GLFWwindow*) handle, &bounds.x, &bounds.y);
        glfwGetWindowSize((GLFWwindow*) handle, &bounds.z, &bounds.w);
        
        return bounds;
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
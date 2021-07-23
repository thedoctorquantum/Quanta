#include "Window.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Quanta
{
    static uint32_t instanceCount;
    
    Window::Window(const std::string& title, glm::ivec2 size)
    {
        instanceCount++;

        if(instanceCount == 1)
        {
            glfwInit();

            glfwSetErrorCallback([](int level, const char* message)
            {
                std::cout << "[GLFW]: " << message << std::endl;
            });
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent((GLFWwindow*) handle);

        gladLoadGL();
        
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

        glDebugMessageCallback([](GLenum source, GLenum type, uint32_t id, GLenum severity, GLsizei length, const char* message, const void* userParam) 
        {
            std::cout << "[OpenGL]: " << message << std::endl;
        }, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
    }

    Window::~Window()
    {
        instanceCount--;

        glfwDestroyWindow((GLFWwindow*) handle);

        if(!instanceCount)
        {
            glfwTerminate();
        }
    }

    glm::ivec2 Window::GetPosition() const
    {
        glm::ivec2 position;

        glfwGetWindowPos((GLFWwindow*) handle, &position.x, &position.y);

        return position;
    }
    
    glm::uvec2 Window::GetSize() const
    {
        glm::uvec2 size;

        glfwGetWindowSize((GLFWwindow*) handle, (int*) &size.x, (int*) &size.y);

        return size;
    }

    glm::ivec4 Window::GetBounds() const
    {
        glm::ivec4 bounds;

        glfwGetWindowPos((GLFWwindow*) handle, &bounds.x, &bounds.y);
        glfwGetWindowSize((GLFWwindow*) handle, &bounds.z, &bounds.w);
        
        return bounds;
    }

    int32_t Window::GetX() const
    {
        int32_t x;

        glfwGetWindowPos((GLFWwindow*) handle, &x, nullptr);

        return x;
    }

    void Window::SetX(int32_t value)
    {
        glfwSetWindowPos((GLFWwindow*) handle, value, GetY());
    }

    int32_t Window::GetY() const
    {
        int32_t y;

        glfwGetWindowPos((GLFWwindow*) handle, nullptr, &y);

        return y;
    }

    void Window::SetY(int32_t value)
    {
        glfwSetWindowPos((GLFWwindow*) handle, GetX(), value);
    }

    uint32_t Window::GetWidth() const
    {
        uint32_t width;

        glfwGetWindowSize((GLFWwindow*) handle, (int*) &width, nullptr);

        return width;
    }

    void Window::SetWidth(uint32_t value) 
    {
        glfwSetWindowSize((GLFWwindow*) handle, value, GetHeight());
    }
    
    uint32_t Window::GetHeight() const
    {
        uint32_t height;

        glfwGetWindowSize((GLFWwindow*) handle, nullptr, (int*) &height);

        return height;
    }

    void Window::SetHeight(uint32_t value) 
    {
        glfwSetWindowSize((GLFWwindow*) handle, GetWidth(), value);
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
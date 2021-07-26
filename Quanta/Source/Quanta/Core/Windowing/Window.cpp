#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

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
                std::cout << "[GLFW]: " << message << '\n';
            });
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer((GLFWwindow*) handle, this);

        glfwSetKeyCallback((GLFWwindow*) handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
        {
            Window* _this = (Window*) glfwGetWindowUserPointer(window);

            switch(action)
            {
            case GLFW_PRESS:
                _this->onKeyDown((Key) key);
                
                break;
            case GLFW_RELEASE:
                _this->onKeyUp((Key) key);

                break;
            }
        });

        glfwSetCharCallback((GLFWwindow*) handle, [](GLFWwindow* window, uint32_t character)
        {
            Window* _this = (Window*) glfwGetWindowUserPointer(window);

            _this->onCharacterDown((char) character);
        });

        glfwSetMouseButtonCallback((GLFWwindow*) handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
        {
            Window* _this = (Window*) glfwGetWindowUserPointer(window);

            switch(action)
            {
            case GLFW_PRESS:
                _this->onMouseDown((MouseButton) button);
                
                break;
            case GLFW_RELEASE:
                _this->onMouseUp((MouseButton) button);

                break;
            }
        });

		glfwSetCursorPosCallback((GLFWwindow*) handle, [](GLFWwindow* window, double xPos, double yPos)
		{
            Window* _this = (Window*) glfwGetWindowUserPointer(window);

            _this->onMouseMove({ (float) xPos, (float) yPos });
		});

        glfwSetScrollCallback((GLFWwindow*) handle, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            Window* _this = (Window*) glfwGetWindowUserPointer(window);

            _this->onMouseScroll({ (float) xOffset, (float) yOffset });
        });

        glfwMakeContextCurrent((GLFWwindow*) handle);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            std::cout << "Failed to load OpenGL\n";
        }
        
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

        glDebugMessageCallback([](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) 
        {
            std::cout << "[OpenGL]: " << message << '\n';
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
    
    void Window::AddKeyDownCallback(Event<Key>::Handler handler)
    {
        onKeyDown += handler;
    }

    void Window::AddKeyUpCallback(Event<Key>::Handler handler)
    {
        onKeyUp += handler;
    }

    void Window::AddMouseDownCallback(Event<MouseButton>::Handler handler)
    {
        onMouseDown += handler;
    }
    
    void Window::AddMouseUpCallback(Event<MouseButton>::Handler handler)
    {
        onMouseUp += handler;
    }

    void Window::AddMouseMoveCallback(Event<glm::vec2>::Handler handler)
    {
        onMouseMove += handler;
    }

    void Window::AddMouseScrollCallback(Event<glm::vec2>::Handler handler)
    {
        onMouseScroll += handler;
    }

    void Window::AddCharacterDownCallback(Event<char>::Handler handler)
    {
        onCharacterDown += handler;
    }

    WindowState Window::GetState() const
    {
        return state;
    }
    
    void Window::SetState(WindowState value)
    {
        state = value;

        switch(value)
        {
        case WindowState::Maximized:
            glfwMaximizeWindow((GLFWwindow*) handle);

            break;
        case WindowState::Minimized:
            
            break;
        default:
            break;
        }
    }

    glm::uvec2 Window::GetPosition() const
    {
        glm::uvec2 position;

        glfwGetWindowPos((GLFWwindow*) handle, (int*) &position.x, (int*) &position.y);

        return position;
    }
    
    glm::uvec2 Window::GetSize() const
    {
        glm::uvec2 size;

        glfwGetWindowSize((GLFWwindow*) handle, (int*) &size.x, (int*) &size.y);

        return size;
    }

    glm::uvec4 Window::GetBounds() const
    {
        glm::uvec4 bounds;

        glfwGetWindowPos((GLFWwindow*) handle, (int*) &bounds.x, (int*) &bounds.y);
        glfwGetWindowSize((GLFWwindow*) handle, (int*) &bounds.z, (int*) &bounds.w);
        
        return bounds;
    }

    uint32_t Window::GetX() const
    {
        uint32_t x;

        glfwGetWindowPos((GLFWwindow*) handle, (int*) &x, nullptr);

        return x;
    }

    void Window::SetX(uint32_t value)
    {
        glfwSetWindowPos((GLFWwindow*) handle, value, GetY());
    }

    uint32_t Window::GetY() const
    {
        int32_t y;

        glfwGetWindowPos((GLFWwindow*) handle, nullptr, &y);

        return y;
    }

    void Window::SetY(uint32_t value)
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
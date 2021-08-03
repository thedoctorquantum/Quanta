#include <iostream>
#include <glad/glad.h>

#include "GlfwWindow.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    static uint32_t instanceCount = 0;

    GlfwWindow::GlfwWindow()
    {
        instanceCount++;

        if(instanceCount == 1)
        {
            bool isInitialized = glfwInit();

            DEBUG_ASSERT(isInitialized);

            glfwSetErrorCallback([](int level, const char* message)
            {
                std::cout << "[GLFW]: " << message << '\n';
            });
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        #ifndef NDEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
        #endif

        handle = glfwCreateWindow(640, 480, "Window", nullptr, nullptr);
        
        DEBUG_ASSERT(handle != nullptr);

        glfwSetWindowUserPointer(handle, this);

        glfwSetKeyCallback(handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
        {
            GlfwWindow* _this = (GlfwWindow*) glfwGetWindowUserPointer(window);

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

        glfwSetCharCallback(handle, [](GLFWwindow* window, uint32_t character)
        {
            GlfwWindow* _this = (GlfwWindow*) glfwGetWindowUserPointer(window);

            _this->onCharacterDown((char) character);
        });

        glfwSetMouseButtonCallback(handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
        {
            GlfwWindow* _this = (GlfwWindow*) glfwGetWindowUserPointer(window);

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

		glfwSetCursorPosCallback(handle, [](GLFWwindow* window, double xPos, double yPos)
		{
            GlfwWindow* _this = (GlfwWindow*) glfwGetWindowUserPointer(window);

            _this->onMouseMove({ (float) xPos, (float) yPos });
		});

        glfwSetScrollCallback(handle, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            GlfwWindow* _this = (GlfwWindow*) glfwGetWindowUserPointer(window);

            _this->onMouseScroll({ (float) xOffset, (float) yOffset });
        });

        glfwMakeContextCurrent(handle);
    }
    
    GlfwWindow::~GlfwWindow() 
    {
        instanceCount--;
        
        glfwDestroyWindow(handle);

        if(!instanceCount)
        {
            glfwTerminate();
        }
    }

    void GlfwWindow::AddKeyDownCallback(Event<Key>::Handler handler)
    {
        onKeyDown += handler;
    }

    void GlfwWindow::AddKeyUpCallback(Event<Key>::Handler handler)
    {
        onKeyUp += handler;
    }

    void GlfwWindow::AddMouseDownCallback(Event<MouseButton>::Handler handler)
    {
        onMouseDown += handler;
    }
    
    void GlfwWindow::AddMouseUpCallback(Event<MouseButton>::Handler handler)
    {
        onMouseUp += handler;
    }

    void GlfwWindow::AddMouseMoveCallback(Event<glm::vec2>::Handler handler)
    {
        onMouseMove += handler;
    }

    void GlfwWindow::AddMouseScrollCallback(Event<glm::vec2>::Handler handler)
    {
        onMouseScroll += handler;
    }

    void GlfwWindow::AddCharacterDownCallback(Event<char>::Handler handler)
    {
        onCharacterDown += handler;
    }

    WindowState GlfwWindow::GetState() const
    {
        return state;
    }
    
    void GlfwWindow::SetState(WindowState value)
    {
        state = value;

        switch(value)
        {
        case WindowState::Maximized:
            glfwMaximizeWindow((GLFWwindow*) handle);

            break;
        case WindowState::Minimized:
            DEBUG_WARNING("WindowState::Minimized is not currently implemented");
            
            break;
        default:
            DEBUG_FAILURE_MESSAGE("value was out of range");
        }
    }

    glm::uvec2 GlfwWindow::GetPosition() const
    {
        glm::uvec2 position;

        glfwGetWindowPos(handle, (int*) &position.x, (int*) &position.y);

        return position;
    }
    
    glm::uvec2 GlfwWindow::GetSize() const
    {
        glm::uvec2 size;

        glfwGetWindowSize(handle, (int*) &size.x, (int*) &size.y);

        return size;
    }

    glm::uvec4 GlfwWindow::GetBounds() const
    {
        glm::uvec4 bounds;

        glfwGetWindowPos(handle, (int*) &bounds.x, (int*) &bounds.y);
        glfwGetWindowSize(handle, (int*) &bounds.z, (int*) &bounds.w);
        
        return bounds;
    }

    uint32_t GlfwWindow::GetX() const
    {
        uint32_t x;

        glfwGetWindowPos(handle, (int*) &x, nullptr);

        return x;
    }

    void GlfwWindow::SetX(uint32_t value)
    {
        glfwSetWindowPos(handle, value, GetY());
    }

    uint32_t GlfwWindow::GetY() const
    {
        int32_t y;

        glfwGetWindowPos(handle, nullptr, &y);

        return y;
    }

    void GlfwWindow::SetY(uint32_t value)
    {
        glfwSetWindowPos(handle, GetX(), value);
    }

    uint32_t GlfwWindow::GetWidth() const
    {
        uint32_t width;

        glfwGetWindowSize(handle, (int*) &width, nullptr);

        return width;
    }

    void GlfwWindow::SetWidth(uint32_t value) 
    {
        glfwSetWindowSize(handle, value, GetHeight());
    }
    
    uint32_t GlfwWindow::GetHeight() const
    {
        uint32_t height;

        glfwGetWindowSize(handle, nullptr, (int*) &height);

        return height;
    }

    void GlfwWindow::SetHeight(uint32_t value) 
    {
        glfwSetWindowSize(handle, GetWidth(), value);
    }
    
    bool GlfwWindow::Exists() const
    {
        return !glfwWindowShouldClose(handle);
    }

    void GlfwWindow::PollEvents() const
    {
        glfwPollEvents();
    }
    
    void GlfwWindow::SwapBuffers() const
    {
        glfwSwapBuffers(handle);
    }
}
#include <iostream>
#include <glad/glad.h>

#include "GlfwWindow.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    GlfwWindow::GlfwWindow(GraphicsApi graphicsApi)
    {
        this->graphicsApi = graphicsApi;

        bool isInitialized = glfwInit();
        
        DEBUG_ASSERT(isInitialized);
        
#if DEBUG
        glfwSetErrorCallback([](int level, const char* message)
        {
            DEBUG_FAILURE_MESSAGE(message);
        });
#endif 
        
        DEBUG_ASSERT(graphicsApi == GraphicsApi::OpenGL);
        
        switch(graphicsApi)
        {
        case GraphicsApi::OpenGL:
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG);

            break;
        }
        
        handle = glfwCreateWindow(640, 480, "Window", nullptr, nullptr);
        
        DEBUG_ASSERT(handle != nullptr);

        if(graphicsApi == GraphicsApi::OpenGL)
        {
            glfwMakeContextCurrent(handle);
        }

        glfwSetWindowUserPointer(handle, this);

        glfwSetKeyCallback(handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
        {
            GlfwWindow* _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

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
            GlfwWindow* _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

            _this->onCharacterDown((char) character);
        });

        glfwSetMouseButtonCallback(handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
        {
            GlfwWindow* _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

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
            GlfwWindow* _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

            _this->onMouseMove({ (float) xPos, (float) yPos });
		});

        glfwSetScrollCallback(handle, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            GlfwWindow* _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

            _this->onMouseScroll({ (float) xOffset, (float) yOffset });
        });
    }
    
    GlfwWindow::~GlfwWindow() 
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
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
        switch(value)
        {
        case WindowState::Maximized:
            glfwMaximizeWindow(handle);

            break;
        case WindowState::Minimized:
                        
            break;
        default:
            DEBUG_FAILURE_MESSAGE("value was out of range");
        }

        state = value;
    }

    glm::uvec2 GlfwWindow::GetPosition() const
    {
        int x;
        int y;

        glfwGetWindowPos(handle, &x, &y);

        return glm::uvec2(x, y);
    }
    
    glm::uvec2 GlfwWindow::GetSize() const
    {
        int x; 
        int y;

        glfwGetWindowSize(handle, &x, &y);

        return glm::uvec2(x, y);
    }

    glm::uvec4 GlfwWindow::GetBounds() const
    {
        int x;
        int y; 
        int width;
        int height;

        glfwGetWindowPos(handle, &x, &y);
        glfwGetWindowSize(handle, &width, &height);
        
        return glm::uvec4(x, y, width, height);
    }

    uint32_t GlfwWindow::GetX() const
    {
        int x;

        glfwGetWindowPos(handle, &x, nullptr);

        return x;
    }

    void GlfwWindow::SetX(uint32_t value)
    {
        glfwSetWindowPos(handle, value, GetY());
    }

    uint32_t GlfwWindow::GetY() const
    {
        int y;

        glfwGetWindowPos(handle, nullptr, &y);

        return y;
    }

    void GlfwWindow::SetY(uint32_t value)
    {
        glfwSetWindowPos(handle, GetX(), value);
    }

    uint32_t GlfwWindow::GetWidth() const
    {
        int width;

        glfwGetWindowSize(handle, &width, nullptr);

        return width;
    }

    void GlfwWindow::SetWidth(uint32_t value) 
    {
        glfwSetWindowSize(handle, value, GetHeight());
    }
    
    uint32_t GlfwWindow::GetHeight() const
    {
        int height;

        glfwGetWindowSize(handle, nullptr, &height);

        return height;
    }

    void GlfwWindow::SetHeight(uint32_t value) 
    {
        glfwSetWindowSize(handle, GetWidth(), value);
    }
    
    const GLFWwindow* GlfwWindow::GetHandle() const
    {
        return handle;
    }

    GLFWwindow* GlfwWindow::GetHandle() 
    {
        return handle;
    }

    GraphicsApi GlfwWindow::GetGraphicsApi() const
    {
        return graphicsApi;
    }
}
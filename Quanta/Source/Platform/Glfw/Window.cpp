#include "Window.h"
#include "../../Debugging/Validation.h"

namespace Quanta::Glfw
{
    Window::Window(GraphicsApi graphicsApi)
    {
        this->graphicsApi = graphicsApi;

        bool isInitialized = glfwInit();
        
        DEBUG_ASSERT(isInitialized);
        
        if constexpr (DEBUG)
        {
            glfwSetErrorCallback([](int level, const char* message)
            {
                DEBUG_FAILURE_MESSAGE(message);
            });
        }

        DEBUG_ASSERT(graphicsApi == GraphicsApi::OpenGL);
        
        switch(graphicsApi)
        {
        case GraphicsApi::OpenGL:
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG);

            glfwWindowHint(GLFW_SAMPLES, 4);

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
            Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

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
            Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onCharacterDown((char) character);
        });

        glfwSetMouseButtonCallback(handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
        {
            Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

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
            Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onMouseMove({ (float) xPos, (float) yPos });
		});

        glfwSetScrollCallback(handle, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onMouseScroll({ (float) xOffset, (float) yOffset });
        });
    }
    
    Window::~Window() 
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }
    
    void Window::Close()
    {
        glfwSetWindowShouldClose(handle, true);
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
        DEBUG_ASSERT(
            value == WindowState::Maximized ||
            value == WindowState::Minimized
        );

        switch(value)
        {
        case WindowState::Maximized:
            glfwMaximizeWindow(handle);

            break;
        case WindowState::Minimized:

            break;
        }

        state = value;
    }

    glm::uvec2 Window::GetPosition() const
    {
        int x;
        int y;

        glfwGetWindowPos(handle, &x, &y);

        return glm::uvec2(x, y);
    }
    
    glm::uvec2 Window::GetSize() const
    {
        int x; 
        int y;

        glfwGetWindowSize(handle, &x, &y);

        return glm::uvec2(x, y);
    }

    glm::uvec4 Window::GetBounds() const
    {
        int x;
        int y; 
        int width;
        int height;

        glfwGetWindowPos(handle, &x, &y);
        glfwGetWindowSize(handle, &width, &height);
        
        return glm::uvec4(x, y, width, height);
    }

    uint32_t Window::GetX() const
    {
        int x;

        glfwGetWindowPos(handle, &x, nullptr);

        return x;
    }

    void Window::SetX(uint32_t value)
    {
        glfwSetWindowPos(handle, value, GetY());
    }

    uint32_t Window::GetY() const
    {
        int y;

        glfwGetWindowPos(handle, nullptr, &y);

        return y;
    }

    void Window::SetY(uint32_t value)
    {
        glfwSetWindowPos(handle, GetX(), value);
    }

    uint32_t Window::GetWidth() const
    {
        int width;

        glfwGetWindowSize(handle, &width, nullptr);

        return width;
    }

    void Window::SetWidth(uint32_t value) 
    {
        glfwSetWindowSize(handle, value, GetHeight());
    }
    
    uint32_t Window::GetHeight() const
    {
        int height;

        glfwGetWindowSize(handle, nullptr, &height);

        return height;
    }

    void Window::SetHeight(uint32_t value) 
    {
        glfwSetWindowSize(handle, GetWidth(), value);
    }

    const std::string& Window::GetTitle() const
    {
        return title;
    }
    
    void Window::SetTitle(const std::string& value)
    {
        title = value;

        glfwSetWindowTitle(handle, title.c_str());
    }

    double Window::GetTime() const
    {
        return glfwGetTime();
    }

    GraphicsApi Window::GetGraphicsApi() const
    {
        return graphicsApi;
    }

    const GLFWwindow* Window::GetHandle() const
    {
        return handle;
    }

    GLFWwindow* Window::GetHandle() 
    {
        return handle;
    }
}
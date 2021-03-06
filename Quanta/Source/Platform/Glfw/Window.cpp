#include <unordered_map>

#include "../../Debugging/Validation.h"
#include "Window.h"

namespace Quanta::Glfw
{
    static const std::unordered_map<int32_t, Key> keyTranslations
    {
        { GLFW_KEY_UNKNOWN, Key::Unknown },
        { GLFW_KEY_SPACE, Key::Space },
        { GLFW_KEY_APOSTROPHE, Key::Apostrophe },
        { GLFW_KEY_COMMA, Key::Comma },
        { GLFW_KEY_MINUS, Key::Minus }, 
        { GLFW_KEY_PERIOD, Key::Period },
        { GLFW_KEY_SLASH, Key::Slash },  
        { GLFW_KEY_0, Key::Number0 },
        { GLFW_KEY_1, Key::Number1 },
        { GLFW_KEY_2, Key::Number2 },
        { GLFW_KEY_3, Key::Number3 },
        { GLFW_KEY_4, Key::Number4 },
        { GLFW_KEY_5, Key::Number5 },
        { GLFW_KEY_6, Key::Number6 },
        { GLFW_KEY_7, Key::Nubmer7 },
        { GLFW_KEY_8, Key::Number8 },
        { GLFW_KEY_9, Key::Number9 },
        { GLFW_KEY_SEMICOLON, Key::Semicolon },
        { GLFW_KEY_EQUAL, Key::Equals }, 
        { GLFW_KEY_A, Key::A },
        { GLFW_KEY_B, Key::B },
        { GLFW_KEY_C, Key::C },
        { GLFW_KEY_D, Key::D },
        { GLFW_KEY_E, Key::E },
        { GLFW_KEY_F, Key::F },
        { GLFW_KEY_G, Key::G },
        { GLFW_KEY_H, Key::H },
        { GLFW_KEY_I, Key::I },
        { GLFW_KEY_J, Key::J },
        { GLFW_KEY_K, Key::K },
        { GLFW_KEY_L, Key::L },
        { GLFW_KEY_M, Key::M },
        { GLFW_KEY_N, Key::N },
        { GLFW_KEY_O, Key::O },
        { GLFW_KEY_P, Key::P },
        { GLFW_KEY_Q, Key::Q },
        { GLFW_KEY_R, Key::R },
        { GLFW_KEY_S, Key::S },
        { GLFW_KEY_T, Key::T },
        { GLFW_KEY_U, Key::U },
        { GLFW_KEY_V, Key::V },
        { GLFW_KEY_W, Key::W },
        { GLFW_KEY_X, Key::X },
        { GLFW_KEY_Y, Key::Y },
        { GLFW_KEY_Z, Key::Z },
        { GLFW_KEY_LEFT_BRACKET, Key::LeftBracket }, 
        { GLFW_KEY_BACKSLASH, Key::BackSlash }, 
        { GLFW_KEY_RIGHT_BRACKET, Key::RightBracket }, 
        { GLFW_KEY_GRAVE_ACCENT, Key::Grave }, 
        { GLFW_KEY_WORLD_1, Key::NonUSNumber1 }, 
        { GLFW_KEY_WORLD_2, Key::NonUSNumber2 }, 
        { GLFW_KEY_ESCAPE, Key::Escape },
        { GLFW_KEY_ENTER, Key::Enter },
        { GLFW_KEY_TAB, Key::Tab },
        { GLFW_KEY_BACKSPACE, Key::Backspace },
        { GLFW_KEY_INSERT, Key::Insert },
        { GLFW_KEY_DELETE, Key::Delete },
        { GLFW_KEY_RIGHT, Key::Right },
        { GLFW_KEY_LEFT, Key::Left },
        { GLFW_KEY_DOWN, Key::Down },
        { GLFW_KEY_UP, Key::Up },
        { GLFW_KEY_PAGE_UP, Key::PageUp },
        { GLFW_KEY_PAGE_DOWN, Key::PageDown },
        { GLFW_KEY_HOME, Key::Home },
        { GLFW_KEY_END, Key::End },
        { GLFW_KEY_CAPS_LOCK, Key::CapsLock },
        { GLFW_KEY_SCROLL_LOCK, Key::ScrollLock },
        { GLFW_KEY_NUM_LOCK, Key::NumLock },
        { GLFW_KEY_PRINT_SCREEN, Key::PrintScreen },
        { GLFW_KEY_PAUSE, Key::Pause },
        { GLFW_KEY_F1, Key::F1 },
        { GLFW_KEY_F2, Key::F2 },
        { GLFW_KEY_F3, Key::F3 },
        { GLFW_KEY_F4, Key::F4 },
        { GLFW_KEY_F5, Key::F5 },
        { GLFW_KEY_F6, Key::F6 },
        { GLFW_KEY_F7, Key::F7 },
        { GLFW_KEY_F8, Key::F8 },
        { GLFW_KEY_F9, Key::F9 },
        { GLFW_KEY_F10, Key::F10 },
        { GLFW_KEY_F11, Key::F11 },
        { GLFW_KEY_F12, Key::F12 },
        { GLFW_KEY_F13, Key::F13 },
        { GLFW_KEY_F14, Key::F14 },
        { GLFW_KEY_F15, Key::F15 },
        { GLFW_KEY_F16, Key::F16 },
        { GLFW_KEY_F17, Key::F17 },
        { GLFW_KEY_F18, Key::F18 },
        { GLFW_KEY_F19, Key::F19 },
        { GLFW_KEY_F20, Key::F20 },
        { GLFW_KEY_F21, Key::F21 },
        { GLFW_KEY_F22, Key::F22 },
        { GLFW_KEY_F23, Key::F23 },
        { GLFW_KEY_F24, Key::F24 },
        { GLFW_KEY_F25, Key::F25 },
        { GLFW_KEY_KP_0, Key::Keypad0 },
        { GLFW_KEY_KP_1, Key::Keypad1 },
        { GLFW_KEY_KP_2, Key::Keypad2 },
        { GLFW_KEY_KP_3, Key::Keypad3 },
        { GLFW_KEY_KP_4, Key::Keypad4 },
        { GLFW_KEY_KP_5, Key::Keypad5 },
        { GLFW_KEY_KP_6, Key::Keypad6 },
        { GLFW_KEY_KP_7, Key::Keypad7 },
        { GLFW_KEY_KP_8, Key::Keypad8 },
        { GLFW_KEY_KP_9, Key::Keypad9 },
        { GLFW_KEY_KP_DECIMAL, Key::KeypadDecimal },
        { GLFW_KEY_KP_DIVIDE, Key::KeypadDivide },
        { GLFW_KEY_KP_MULTIPLY, Key::KeypadMultiply },
        { GLFW_KEY_KP_SUBTRACT, Key::KeypadSubtract },
        { GLFW_KEY_KP_ADD, Key::KeypadAdd },
        { GLFW_KEY_KP_ENTER, Key::KeypadEnter },
        { GLFW_KEY_KP_EQUAL, Key::KeypadEquals },
        { GLFW_KEY_LEFT_SHIFT, Key::LeftShift },
        { GLFW_KEY_LEFT_CONTROL, Key::LeftControl },
        { GLFW_KEY_LEFT_ALT, Key::LeftAlt },
        { GLFW_KEY_LEFT_SUPER, Key::LeftSuper },
        { GLFW_KEY_RIGHT_SHIFT, Key::LeftShift },
        { GLFW_KEY_RIGHT_CONTROL, Key::RightControl },
        { GLFW_KEY_RIGHT_ALT, Key::RightAlt },
        { GLFW_KEY_RIGHT_SUPER, Key::RightSuper },
        { GLFW_KEY_MENU, Key::Menu }
    };

    static const std::unordered_map<int32_t, MouseButton> mouseButtonTranslations
    {
        { GLFW_MOUSE_BUTTON_1, MouseButton::One },
        { GLFW_MOUSE_BUTTON_2, MouseButton::Two },
        { GLFW_MOUSE_BUTTON_3, MouseButton::Three },
        { GLFW_MOUSE_BUTTON_4, MouseButton::Four },
        { GLFW_MOUSE_BUTTON_5, MouseButton::Five },
        { GLFW_MOUSE_BUTTON_6, MouseButton::Six },
        { GLFW_MOUSE_BUTTON_7, MouseButton::Seven },
        { GLFW_MOUSE_BUTTON_8, MouseButton::Eight },
        { GLFW_MOUSE_BUTTON_LAST, MouseButton::Last },
        { GLFW_MOUSE_BUTTON_LEFT, MouseButton::Left },
        { GLFW_MOUSE_BUTTON_RIGHT, MouseButton::Right },
        { GLFW_MOUSE_BUTTON_MIDDLE, MouseButton::Middle }
    };

    Window::Window(const GraphicsApi graphicsApi)
    {
        this->graphicsApi = graphicsApi;

        const auto isInitialized = glfwInit();
        
        DEBUG_ASSERT(isInitialized);
        
        if constexpr (DEBUG)
        {
            glfwSetErrorCallback([](const auto level, const auto message)
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

        glfwWindowHint(GLFW_MAXIMIZED, true);

        handle = glfwCreateWindow(size.x, size.y, "Window", nullptr, nullptr);
        
        DEBUG_ASSERT(handle != nullptr);

        if(graphicsApi == GraphicsApi::OpenGL)
        {
            glfwMakeContextCurrent(handle);
        }
        
        glfwSetWindowUserPointer(handle, this);
        
        glfwSetKeyCallback(handle, [](const auto window, const auto key, const auto scancode, const auto action, const auto mods)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
            
            Key result = keyTranslations.at(key);

            switch(action)
            {
            case GLFW_PRESS:
                _this->onKeyDown(result);
                
                break;
            case GLFW_RELEASE:
                _this->onKeyUp(result);

                break;
            }
        });

        glfwSetCharCallback(handle, [](const auto window, const auto character)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onCharacterDown(static_cast<char>(character));
        });

        glfwSetMouseButtonCallback(handle, [](const auto window, const auto button, const auto action, const auto mods)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            const auto result = mouseButtonTranslations.at(button);

            switch(action)
            {
            case GLFW_PRESS:
                _this->onMouseDown(result);
                
                break;
            case GLFW_RELEASE:
                _this->onMouseUp(result);

                break;
            }
        });

		glfwSetCursorPosCallback(handle, [](const auto window, const auto xPos, const auto yPos)
		{
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onMouseMove({ static_cast<float>(xPos), static_cast<float>(yPos) });
		});

        glfwSetScrollCallback(handle, [](const auto window, const auto xOffset, const auto yOffset)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->onMouseScroll({ static_cast<float>(xOffset), static_cast<float>(yOffset) });
        });

        glfwSetWindowPosCallback(handle, [](const auto window, const auto x, const auto y)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

			_this->position.x = static_cast<std::uint32_t>(x);
            _this->position.y = static_cast<std::uint32_t>(y);
        });

		glfwSetWindowSizeCallback(handle, [](const auto window, const auto width, const auto height)
		{
			const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

			_this->size.x = static_cast<std::uint32_t>(width);
            _this->size.y = static_cast<std::uint32_t>(height);
		});

        glfwSetWindowMaximizeCallback(handle, [](const auto window, const auto maximized)
        {
            const auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

            _this->position.x = 0;
            _this->position.y = 0;

            int width = 0;
            int height = 0;

            glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &width, &height);

            _this->size.x = width;
            _this->size.y = height;
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

    void Window::AddKeyDownCallback(const Event<Key>::Handler handler)
    {
        onKeyDown += handler;
    }

    void Window::AddKeyUpCallback(const Event<Key>::Handler handler)
    {
        onKeyUp += handler;
    }

    void Window::AddMouseDownCallback(const Event<MouseButton>::Handler handler)
    {
        onMouseDown += handler;
    }
    
    void Window::AddMouseUpCallback(const Event<MouseButton>::Handler handler)
    {
        onMouseUp += handler;
    }

    void Window::AddMouseMoveCallback(const Event<glm::vec2>::Handler handler)
    {
        onMouseMove += handler;
    }

    void Window::AddMouseScrollCallback(const Event<glm::vec2>::Handler handler)
    {
        onMouseScroll += handler;
    }

    void Window::AddCharacterDownCallback(const Event<char>::Handler handler)
    {
        onCharacterDown += handler;
    }

    WindowState Window::GetState() const
    {
        return state;
    }
    
    void Window::SetState(const WindowState value)
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
    
    glm::uvec2 Window::GetFrameBufferSize() const
    {
        int width = 0;
        int height = 0;

        glfwGetFramebufferSize(handle, &width, &height);

        return glm::uvec2(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height));
    }

    glm::uvec2 Window::GetPosition() const
    {
        return position;
    }
    
    glm::uvec2 Window::GetSize() const
    {
        return size;
    }

    glm::uvec4 Window::GetBounds() const
    {
        return glm::uvec4(position, size);
    }

    std::uint32_t Window::GetX() const
    {
        return position.x; 
    }

    void Window::SetX(const uint32_t value)
    {
        glfwSetWindowPos(handle, value, GetY());
    }

    uint32_t Window::GetY() const
    {
        return position.y;
    }

    void Window::SetY(const std::uint32_t value)
    {
        glfwSetWindowPos(handle, GetX(), value);
    }

    std::uint32_t Window::GetWidth() const
    {
        return size.x;
    }

    void Window::SetWidth(const std::uint32_t value) 
    {
        glfwSetWindowSize(handle, value, GetHeight());
    }
    
    std::uint32_t Window::GetHeight() const
    {
        return size.y;
    }

    void Window::SetHeight(const uint32_t value) 
    {
        glfwSetWindowSize(handle, GetWidth(), value);
    }
    
    const char* Window::GetClipboardText() const
    {
        return glfwGetClipboardString(handle);
    }
    
    void Window::SetClipboardText(const char* const value) 
    {
        glfwSetClipboardString(handle, value);
    }

    const std::string_view& Window::GetTitle() const
    {
        return title;
    }
    
    void Window::SetTitle(const std::string_view& value)
    {
        title = value;

        glfwSetWindowTitle(handle, title.data());
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
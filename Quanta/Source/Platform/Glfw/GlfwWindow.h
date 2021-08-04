#pragma once

#include <Quanta/Core/Windowing/Window.h>
#include <GLFW/glfw3.h>

namespace Quanta
{
    class GlfwWindow final : public Window
    {
    public:
        GlfwWindow(GraphicsApi graphicsApi);
        ~GlfwWindow();

        GlfwWindow(const GlfwWindow&) = delete;

        GlfwWindow& operator=(const GlfwWindow&) = delete;

        bool Exists() const override;
        void PollEvents() const override;
        void SwapBuffers() const override;

        void AddKeyDownCallback(Event<Key>::Handler handler) override;
        void AddKeyUpCallback(Event<Key>::Handler handler) override;

        void AddMouseDownCallback(Event<MouseButton>::Handler handler) override;
        void AddMouseUpCallback(Event<MouseButton>::Handler handler) override;

        void AddMouseMoveCallback(Event<glm::vec2>::Handler handler) override;
        void AddMouseScrollCallback(Event<glm::vec2>::Handler handler) override;

        void AddCharacterDownCallback(Event<char>::Handler handler) override;
        
        WindowState GetState() const override;
        void SetState(WindowState value) override;
        
        glm::uvec2 GetPosition() const override;
        glm::uvec2 GetSize() const override;
        
        glm::uvec4 GetBounds() const override;

        uint32_t GetX() const override;
        void SetX(uint32_t value) override;

        uint32_t GetY() const override;
        void SetY(uint32_t value) override;

        uint32_t GetWidth() const override;
        void SetWidth(uint32_t value) override;
        
        uint32_t GetHeight() const override;
        void SetHeight(uint32_t value) override;

        GraphicsApi GetGraphicsApi() const override;
        
        const GLFWwindow* GetHandle() const;
        GLFWwindow* GetHandle();
    private:
        GLFWwindow* handle;
        GraphicsApi graphicsApi;

        WindowState state;

        Event<Key> onKeyDown;
        Event<Key> onKeyUp;

        Event<MouseButton> onMouseDown;
        Event<MouseButton> onMouseUp;

        Event<glm::vec2> onMouseMove;
        Event<glm::vec2> onMouseScroll;

        Event<char> onCharacterDown; 
    };
}
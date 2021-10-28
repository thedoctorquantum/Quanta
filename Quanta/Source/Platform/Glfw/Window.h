#pragma once

#include <Quanta/Core/Windowing/Window.h>
#include <GLFW/glfw3.h>

namespace Quanta::Glfw
{
    struct Window : public Quanta::Window
    {
        Window(GraphicsApi graphicsApi);
        ~Window();

        Window(const Window&) = delete;

        Window& operator=(const Window&) = delete;
        
        void Close() override;
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
        
        glm::uvec2 GetFrameBufferSize() const override;

        glm::uvec2 GetPosition() const override;
        glm::uvec2 GetSize() const override;
        
        glm::uvec4 GetBounds() const override;

        uint32_t GetX() const override;
        void SetX(uint32_t) override;

        uint32_t GetY() const override;
        void SetY(uint32_t) override;

        uint32_t GetWidth() const override;
        void SetWidth(uint32_t) override;
        
        uint32_t GetHeight() const override;
        void SetHeight(uint32_t) override;

        const char* GetClipboardText() const override;
        void SetClipboardText(const char*) override;

        const std::string& GetTitle() const override;
        void SetTitle(const std::string&) override;

        double GetTime() const override; 

        GraphicsApi GetGraphicsApi() const override;
        
        const GLFWwindow* GetHandle() const;
        GLFWwindow* GetHandle();
    private:
        GLFWwindow* handle = nullptr;
        GraphicsApi graphicsApi;

        std::string title;

        glm::uvec2 position { 0u };
        glm::uvec2 size { 640u, 480u };

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
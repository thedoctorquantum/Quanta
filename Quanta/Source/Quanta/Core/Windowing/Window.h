#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <functional>

#include "WindowState.h"
#include "../Event.h"
#include "../../Input/Key.h"
#include "../../Input/MouseButton.h"

namespace Quanta
{
    class Window final
    {
    public:
        Window(const std::string& title, glm::ivec2 size);
        ~Window();
        
        Window(const Window&) = delete;

        Window& operator=(const Window&) = delete;

        void AddKeyDownCallback(Event<Key>::Handler handler);
        void AddKeyUpCallback(Event<Key>::Handler handler);

        void AddMouseDownCallback(Event<MouseButton>::Handler handler);
        void AddMouseUpCallback(Event<MouseButton>::Handler handler);

        void AddMouseMoveCallback(Event<glm::vec2>::Handler handler);
        void AddMouseScrollCallback(Event<glm::vec2>::Handler handler);

        void AddCharacterDownCallback(Event<char>::Handler handler);
        
        WindowState GetState() const;
        void SetState(WindowState value);
        
        glm::uvec2 GetPosition() const;
        glm::uvec2 GetSize() const;
        
        glm::uvec4 GetBounds() const;

        uint32_t GetX() const;
        void SetX(uint32_t value);

        uint32_t GetY() const;
        void SetY(uint32_t value);

        uint32_t GetWidth() const;
        void SetWidth(uint32_t value);
        
        uint32_t GetHeight() const;
        void SetHeight(uint32_t value);

        bool Exists() const;
        void PollEvents() const;
        void SwapBuffers() const;
    private:
        void* handle;

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
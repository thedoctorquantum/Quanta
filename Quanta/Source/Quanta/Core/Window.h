#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>

namespace Quanta
{
    class Window final
    {
    public:
        Window(const std::string& title, glm::ivec2 size);
        ~Window();

        glm::ivec2 GetPosition() const;
        glm::uvec2 GetSize() const;
        
        glm::ivec4 GetBounds() const;

        int32_t GetX() const;
        void SetX(int32_t value);

        int32_t GetY() const;
        void SetY(int32_t value);

        uint32_t GetWidth() const;
        void SetWidth(uint32_t value);
        
        uint32_t GetHeight() const;
        void SetHeight(uint32_t value);

        bool Exists() const;
        void PollEvents() const;
        void SwapBuffers() const;
    private:
        void* handle;
    };
}
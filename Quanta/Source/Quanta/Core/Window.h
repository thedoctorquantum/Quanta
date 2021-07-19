#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Quanta
{
    class Window final
    {
    public:
        Window(const std::string& title, glm::ivec2 size);
        ~Window();

        glm::ivec2 GetPosition() const;
        glm::ivec2 GetSize() const;
        
        glm::ivec4 GetBounds() const;

        bool Exists() const;
        void PollEvents() const;
        void SwapBuffers() const;
    private:
        void* handle;
    };
}
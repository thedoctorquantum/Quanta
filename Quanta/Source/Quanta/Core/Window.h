#pragma once

#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>

namespace Quanta
{
    class Window final
    {
    public:
        Window(const std::string& title, glm::ivec2 size);
        ~Window();

        bool Exists() const;
        void PollEvents() const;
        void SwapBuffers() const;
    private:
        GLFWwindow* handle;
    };
}
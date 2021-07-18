#include <iostream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <imgui.h>
#include <GLFW/glfw3.h>

int main(int argc, char** args)
{
    Quanta::Log("Hello Sandbox!");

    if(!glfwInit())
    {
        return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Quanta", nullptr, nullptr);

    if(!window)
    {
        glfwTerminate();

        return -1;
    }

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
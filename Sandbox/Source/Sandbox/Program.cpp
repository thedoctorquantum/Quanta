#include <iostream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <imgui.h>

using namespace Quanta;

int main(int argc, char** args)
{
    Window window = Window("Sandbox", { 640, 480 });
        
    while(window.Exists())
    {
        window.PollEvents();

        GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 1.0f, 1.0f }, 1.0f, 0);
        GraphicsDevice::Viewport({ 0, 0, 640, 480 });
        
        window.SwapBuffers();
    }

    return 0;
}
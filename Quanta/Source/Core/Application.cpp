#include <Quanta/Core/Application.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <Quanta/Audio/AudioDevice.h>

namespace Quanta
{
    Application::Application()
    {
        window = Window::Create(GraphicsApi::OpenGL);

        Quanta::GraphicsDevice::Create(*window);
        Quanta::AudioDevice::Create(Quanta::AudioApi::OpenAL);
    }
    
    Application::~Application()
    {
        Quanta::GraphicsDevice::Destroy();
        Quanta::AudioDevice::Destroy();
    }

    void Application::Run()
    {
        double time = 0;
        double lastFrameTime = 0;
        double frameTime = 0;

        while(window->Exists())
        {
            window->PollEvents();

            time = window->GetTime();
            frameTime = time - lastFrameTime;
            lastFrameTime = time;

            OnUpdate(frameTime);

            window->SwapBuffers();
        }        
    }
}
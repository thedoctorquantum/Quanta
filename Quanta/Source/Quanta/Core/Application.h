#pragma once

#include <memory>
#include "Window.h"

namespace Quanta
{
    class Application 
    {
    public:
        Application();

        virtual ~Application() = 0;

        void Run();
    protected:
        virtual void Render(float elapsed) { }
        virtual void Update(float elapsed) { }
        
        std::shared_ptr<Window> window;
    };
}
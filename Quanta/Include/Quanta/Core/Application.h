#pragma once

#include "Windowing/Window.h"

namespace Quanta
{
    class Application 
    {
    public:
        Application();
        virtual ~Application(); 

        void Run();
    protected:
        std::shared_ptr<Window> window;

        virtual void OnUpdate(float frameTime) = 0;
    };
}
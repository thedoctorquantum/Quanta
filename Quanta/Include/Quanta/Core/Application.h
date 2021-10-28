#pragma once

#include "Windowing/Window.h"

namespace Quanta
{
    struct Application 
    {
        Application();
        virtual ~Application(); 

        void Run();
    protected:
        std::shared_ptr<Window> window;

        virtual void OnUpdate(float frameTime) = 0;
    };
}
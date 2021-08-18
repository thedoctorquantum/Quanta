#include <Quanta/Core/Application.h>
#include <Quanta/Graphics/GraphicsDevice.h>
#include <iostream>

#include "Editor.h"

int main()
{   
    auto editor = std::make_unique<Quanta::Editor>();

    editor->Run();

    return 0;
}
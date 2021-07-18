#include "Quanta.h"

#include <iostream>

namespace Quanta
{
    void Log(const std::string& message)
    {
        std::cout << "[Quanta]: " << message << std::endl;
    }
}
#pragma once

namespace Quanta
{
    class CameraComponent final
    {
    public:
        float width = 0;
        float height = 0;
        float far = 1000.0f;
        float near = 0.1f;
        float fieldOfView = 45.0f;
    };
}
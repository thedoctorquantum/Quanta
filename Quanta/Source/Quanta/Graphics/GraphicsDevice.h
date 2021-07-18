#pragma once

#include <glm/glm.hpp>

namespace Quanta
{
    class GraphicsDevice final
    {
    public:
        GraphicsDevice() = delete;
        ~GraphicsDevice() = delete;

        GraphicsDevice(const GraphicsDevice& other) = delete;
        GraphicsDevice(const GraphicsDevice&& other) = delete;

        static void ClearBackBuffer(const glm::vec4& color, float depth, int stencil);
        static void Viewport(const glm::ivec4& viewport);
    };
}
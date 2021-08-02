#pragma once

#include <Quanta/Graphics/Texture/CubeMap.h>

namespace Quanta
{
    class OpenGLCubeMap final : public CubeMap
    {
    public:
        OpenGLCubeMap(size_t width, size_t height);
        ~OpenGLCubeMap();

        void SetData(const void* data, size_t index) override;
        void GetData(void* data, size_t index) const override;
        
        size_t GetWidth() const override;
        size_t GetHeight() const override;

        uint32_t GetHandle() const;
    private:
        size_t width = 0;
        size_t height = 0;

        uint32_t handle = 0;
    };
}
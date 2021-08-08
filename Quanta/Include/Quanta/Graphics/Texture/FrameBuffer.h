#pragma once

#include <memory>

namespace Quanta
{
    class FrameBuffer 
    {
    public:
        static std::shared_ptr<FrameBuffer> Create(size_t width, size_t height);

        virtual ~FrameBuffer() = 0;
    };
}
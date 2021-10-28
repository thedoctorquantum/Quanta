#pragma once

#include <cstdint>

namespace Quanta
{
    enum struct SoundFormat : uint8_t
    {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16,
        TwoDimentional16,
        ThreeDimentional16
    };
}
#pragma once

#include <memory>
#include <string>

#include "SoundFormat.h"

namespace Quanta
{
    struct AudioBuffer
    {
        static std::shared_ptr<AudioBuffer> Create();

        static std::shared_ptr<AudioBuffer> FromFile(const std::string& filepath);

        virtual ~AudioBuffer() = default;

        virtual void SetData(const void* data, size_t size, size_t frequency, SoundFormat format) = 0;

        virtual size_t GetSize() const = 0;
        virtual size_t GetFrequency() const = 0;
        virtual SoundFormat GetFormat() const = 0;
    };
}
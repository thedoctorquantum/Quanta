#pragma once

#include <Quanta/Audio/AudioBuffer.h>
#include <al.h>

namespace Quanta::OpenAL
{
    struct AudioBuffer : public Quanta::AudioBuffer
    {
        AudioBuffer();
        ~AudioBuffer();

        AudioBuffer(const AudioBuffer&) = delete;

        AudioBuffer& operator=(const AudioBuffer&) = delete;

        void SetData(const void* data, size_t size, size_t frequency, SoundFormat format) override;

        size_t GetSize() const override;
        size_t GetFrequency() const override;
        SoundFormat GetFormat() const override;
        
        uint32_t GetHandle() const;
        ALenum GetOpenALFormat() const;
    private:
        size_t size = 0;
        size_t frequency = 0;
        SoundFormat format = SoundFormat::Mono8;

        uint32_t handle = 0;
        ALenum openalFormat = 0;
    };
}
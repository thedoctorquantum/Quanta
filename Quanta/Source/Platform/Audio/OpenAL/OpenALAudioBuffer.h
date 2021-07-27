#pragma once

#include <al.h>

#include "../../../Quanta/Audio/AudioBuffer.h"

namespace Quanta
{
    class OpenALAudioBuffer final : public AudioBuffer
    {
    public:
        OpenALAudioBuffer();
        ~OpenALAudioBuffer();

        OpenALAudioBuffer(const OpenALAudioBuffer&) = delete;

        OpenALAudioBuffer& operator=(const OpenALAudioBuffer&) = delete;

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
#pragma once

#include <Quanta/Audio/AudioDevice.h>
#include <al.h>
#include <AL/alc.h>

namespace Quanta
{
    class OpenALAudioDevice final : public AudioDevice
    {
    public:
        OpenALAudioDevice();
        ~OpenALAudioDevice();

        OpenALAudioDevice(const OpenALAudioDevice&) = delete;
        OpenALAudioDevice& operator=(const OpenALAudioDevice&) = delete;
    private:
        ALCcontext* context;
        ALCdevice* device;

        void InternalPlay(const AudioSource& source, const AudioBuffer& buffer) override;
    };
}
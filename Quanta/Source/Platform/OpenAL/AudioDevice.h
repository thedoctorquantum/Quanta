#pragma once

#include <Quanta/Audio/AudioDevice.h>
#include <al.h>
#include <AL/alc.h>

namespace Quanta::OpenAL
{
    struct AudioDevice : public Quanta::AudioDevice
    {
        AudioDevice();
        ~AudioDevice();

        AudioDevice(const AudioDevice&) = delete;

        AudioDevice& operator=(const AudioDevice&) = delete;
    private:
        ALCcontext* context = nullptr;
        ALCdevice* device = nullptr;

        void InternalPlay(const Quanta::AudioSource& source, const Quanta::AudioBuffer& buffer) override;
    };
}
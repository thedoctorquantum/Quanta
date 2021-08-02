#include <Quanta/Audio/AudioDevice.h>

#include "../Platform/OpenAL/OpenALAudioDevice.h"

namespace Quanta
{
    struct State
    {
        AudioApi api;
        AudioDevice* device;
    } static state;

    void AudioDevice::Initialize(AudioApi api)
    {
        switch(api)
        {
        case AudioApi::OpenAL:
            state.device = new OpenALAudioDevice();

            break;
        }
    }

    void AudioDevice::DeInitialize()
    {
        delete state.device;
    }
    
    AudioApi AudioDevice::GetApi()
    {
        return state.api;
    }

    void AudioDevice::Play(const AudioSource& source, const AudioBuffer& buffer)
    {
        state.device->InternalPlay(source, buffer);
    }
}
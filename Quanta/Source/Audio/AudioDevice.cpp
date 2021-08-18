#include <Quanta/Audio/AudioDevice.h>

#include "../Platform/OpenAL/AudioDevice.h"
#include "../Debugging/Validation.h"

namespace Quanta
{
    struct State
    {
        AudioApi api = AudioApi::OpenAL;
        AudioDevice* device = nullptr;
    } static state;

    void AudioDevice::Create(AudioApi api)
    {
        DEBUG_ASSERT(api == AudioApi::OpenAL);

        switch(api)
        {
        case AudioApi::OpenAL:
            state.device = new OpenAL::AudioDevice();

            break;
        }
    }

    void AudioDevice::Destroy()
    {
        delete state.device;
    }
    
    AudioApi AudioDevice::GetApi()
    {
        return state.api;
    }
    
    void AudioDevice::Play(const AudioSource& source, const AudioBuffer& buffer)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalPlay(source, buffer);
    }
}
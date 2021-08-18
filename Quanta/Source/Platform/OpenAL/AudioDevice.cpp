#include <al.h>
#include <AL/alc.h>

#include "AudioDevice.h"
#include "AudioSource.h"
#include "AudioBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenAL
{
    AudioDevice::AudioDevice()
    {
        device = alcOpenDevice(nullptr);

        DEBUG_ASSERT(device != nullptr);

        context = alcCreateContext(device, nullptr);

        DEBUG_ASSERT(context != nullptr);

        alcMakeContextCurrent(context);
    }
    
    AudioDevice::~AudioDevice()
    {
        alcCloseDevice(device);
        alcDestroyContext(context);
    }
    
    void AudioDevice::InternalPlay(const Quanta::AudioSource& source, const Quanta::AudioBuffer& buffer)
    {
        const AudioSource* alSource = nullptr;
        const AudioBuffer* alBuffer = nullptr;
        
        if constexpr (DEBUG)
        {
            alSource = dynamic_cast<const AudioSource*>(&source);
            alBuffer = dynamic_cast<const AudioBuffer*>(&buffer);

            DEBUG_ASSERT(alSource != nullptr);
            DEBUG_ASSERT(alBuffer != nullptr);
        }
        else
        {
            alSource = static_cast<const AudioSource*>(&source);
            alBuffer = static_cast<const AudioBuffer*>(&buffer);
        }
        
        alSourcei(alSource->GetHandle(), AL_BUFFER, alBuffer->GetHandle());

        alSourcePlay(alSource->GetHandle());
    }
}
#include <al.h>
#include <AL/alc.h>

#include "OpenALAudioDevice.h"
#include "OpenALAudioSource.h"
#include "OpenALAudioBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenALAudioDevice::OpenALAudioDevice()
    {
        device = alcOpenDevice(nullptr);

        DEBUG_ASSERT(device != nullptr);

        context = alcCreateContext(device, nullptr);

        DEBUG_ASSERT(context != nullptr);

        alcMakeContextCurrent(context);
    }
    
    OpenALAudioDevice::~OpenALAudioDevice()
    {
        alcCloseDevice(device);
        alcDestroyContext(context);
    }
    
    void OpenALAudioDevice::InternalPlay(const AudioSource& source, const AudioBuffer& buffer)
    {
        const OpenALAudioSource* alSource = nullptr;
        const OpenALAudioBuffer* alBuffer = nullptr;
        
#if DEBUG
        alSource = dynamic_cast<const OpenALAudioSource*>(&source);
        alBuffer = dynamic_cast<const OpenALAudioBuffer*>(&buffer);

        DEBUG_ASSERT(alSource != nullptr);
        DEBUG_ASSERT(alBuffer != nullptr);
#else
        alSource = static_cast<const OpenALAudioSource*>(&source);
        alBuffer = static_cast<const OpenALAudioBuffer*>(&buffer);
#endif

        alSourcei(alSource->GetHandle(), AL_BUFFER, alBuffer->GetHandle());

        alSourcePlay(alSource->GetHandle());
    }
}
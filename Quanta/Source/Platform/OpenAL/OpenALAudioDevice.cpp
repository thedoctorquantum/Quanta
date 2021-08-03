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
        const OpenALAudioSource* alSource = (const OpenALAudioSource*) &source;
        const OpenALAudioBuffer* alBuffer = (const OpenALAudioBuffer*) &buffer;

        DEBUG_ASSERT(alSource != nullptr);
        DEBUG_ASSERT(alBuffer != nullptr);

        alSourcei(alSource->GetHandle(), AL_BUFFER, alBuffer->GetHandle());

        alSourcePlay(alSource->GetHandle());
    }
}
#include <al.h>
#include <AL/alc.h>

#include "OpenALAudioDevice.h"
#include "OpenALAudioSource.h"
#include "OpenALAudioBuffer.h"

namespace Quanta
{
    OpenALAudioDevice::OpenALAudioDevice()
    {
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);

        alcMakeContextCurrent(context);
    }
    
    OpenALAudioDevice::~OpenALAudioDevice()
    {
        alcCloseDevice(device);
        alcDestroyContext(context);
    }
    
    void OpenALAudioDevice::InternalPlay(const AudioSource& source, const AudioBuffer& buffer)
    {
        const OpenALAudioSource& alSource = (const OpenALAudioSource&) source;
        const OpenALAudioBuffer& alBuffer = (const OpenALAudioBuffer&) buffer;

        alSourcei(alSource.GetHandle(), AL_BUFFER, alBuffer.GetHandle());

        alSourcePlay(alSource.GetHandle());
    }
}
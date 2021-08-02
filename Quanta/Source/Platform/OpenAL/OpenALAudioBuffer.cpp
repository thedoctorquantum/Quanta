#include <AL/alc.h>

#include "OpenALAudioBuffer.h"

namespace Quanta
{
    OpenALAudioBuffer::OpenALAudioBuffer()
    {
        alGenBuffers(1, &handle);
    }

    OpenALAudioBuffer::~OpenALAudioBuffer()
    {
        alDeleteBuffers(1, &handle);
    }

    void OpenALAudioBuffer::SetData(const void* data, size_t size, size_t frequency, SoundFormat format)
    {
        this->size = size;
        this->frequency = frequency;
        
        switch(format)
        {
        case SoundFormat::Mono8:
            openalFormat = AL_FORMAT_MONO8;

            break;
        case SoundFormat::Mono16:
            openalFormat = AL_FORMAT_MONO16;

            break;
        case SoundFormat::Stereo8:
            openalFormat = AL_FORMAT_STEREO8;

            break;
        case SoundFormat::Stereo16:
            openalFormat = AL_FORMAT_STEREO16;

            break;
        }

        alBufferData(handle, openalFormat, data, size, frequency);
    }

    size_t OpenALAudioBuffer::GetSize() const
    {
        return size;
    }

    size_t OpenALAudioBuffer::GetFrequency() const
    {
        return frequency;
    }

    SoundFormat OpenALAudioBuffer::GetFormat() const
    {
        return format;
    }
    
    uint32_t OpenALAudioBuffer::GetHandle() const
    {
        return handle;
    }
}
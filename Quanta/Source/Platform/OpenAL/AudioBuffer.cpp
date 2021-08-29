#include <AL/alc.h>

#include "AudioBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenAL
{
    AudioBuffer::AudioBuffer()
    {
        alGenBuffers(1, &handle);

        DEBUG_ASSERT(handle != 0);
    }

    AudioBuffer::~AudioBuffer()
    {
        alDeleteBuffers(1, &handle);
    }
    
    void AudioBuffer::SetData(const void* const data, const size_t size, const size_t frequency, const SoundFormat format)
    {
        DEBUG_ASSERT(data != nullptr);
        DEBUG_ASSERT(size != 0);
        DEBUG_ASSERT(frequency != 0);
        DEBUG_ASSERT(
            format == SoundFormat::Mono8 ||
            format == SoundFormat::Mono16 ||
            format == SoundFormat::Stereo8 ||
            format == SoundFormat::Stereo16
        );

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
        
        this->format = format;

        alBufferData(handle, openalFormat, data, size, frequency);
    }
    
    size_t AudioBuffer::GetSize() const
    {
        return size;
    }

    size_t AudioBuffer::GetFrequency() const
    {
        return frequency;
    }

    SoundFormat AudioBuffer::GetFormat() const
    {
        return format;
    }
    
    uint32_t AudioBuffer::GetHandle() const
    {
        return handle;
    }
}
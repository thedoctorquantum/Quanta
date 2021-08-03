#include <AL/alc.h>

#include "OpenALAudioBuffer.h"
#include "../../Debugging/Validation.h"

namespace Quanta
{
    OpenALAudioBuffer::OpenALAudioBuffer()
    {
        alGenBuffers(1, &handle);

        DEBUG_ASSERT(handle != 0);
    }

    OpenALAudioBuffer::~OpenALAudioBuffer()
    {
        alDeleteBuffers(1, &handle);
    }
    
    void OpenALAudioBuffer::SetData(const void* data, size_t size, size_t frequency, SoundFormat format)
    {
        DEBUG_ASSERT(data != nullptr);
        DEBUG_ASSERT(size != 0);
        DEBUG_ASSERT(frequency != 0);
        
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
        default:
            DEBUG_FAILURE_MESSAGE("format was out of range");
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
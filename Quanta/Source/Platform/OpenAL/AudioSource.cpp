#include <al.h>
#include <alc.h>

#include "AudioSource.h"
#include "../../Debugging/Validation.h"

namespace Quanta::OpenAL
{
    AudioSource::AudioSource()
    {
        alGenSources(1, &handle);

        DEBUG_ASSERT(handle != 0);
    }

    AudioSource::~AudioSource()
    {
        alDeleteSources(1, &handle);
    }

    const glm::vec3& AudioSource::GetTranslation() const
    {
        return translation;
    }
    
    void AudioSource::SetTranslation(const glm::vec3& value)
    {
        translation = value;

        alSource3f(handle, AL_POSITION, value.x, value.y, value.z);
    }

    uint32_t AudioSource::GetHandle() const
    {
        return handle;
    }
}

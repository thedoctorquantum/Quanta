#include <al.h>
#include <alc.h>

#include "OpenALAudioSource.h"

namespace Quanta
{
    OpenALAudioSource::OpenALAudioSource()
    {
        alGenSources(1, &handle);
    }

    OpenALAudioSource::~OpenALAudioSource()
    {
        alDeleteSources(1, &handle);
    }

    const glm::vec3& OpenALAudioSource::GetTranslation() const
    {
        return translation;
    }
    
    void OpenALAudioSource::SetTranslation(const glm::vec3& value)
    {
        translation = value;

        alSource3f(handle, AL_POSITION, value.x, value.y, value.z);
    }

    uint32_t OpenALAudioSource::GetHandle() const
    {
        return handle;
    }
}

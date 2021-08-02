#include <Quanta/Audio/AudioSource.h>
#include <Quanta/Audio/AudioDevice.h>

#include "../Platform/Audio/OpenAL/OpenALAudioSource.h"

namespace Quanta
{
    std::shared_ptr<AudioSource> AudioSource::Create()
    {
        switch(AudioDevice::GetApi())
        {
        case AudioApi::OpenAL:
            return std::make_shared<OpenALAudioSource>();
        }

        return nullptr;
    }
}
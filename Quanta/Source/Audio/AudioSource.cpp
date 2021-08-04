#include <Quanta/Audio/AudioSource.h>
#include <Quanta/Audio/AudioDevice.h>

#include "../Platform/OpenAL/OpenALAudioSource.h"
#include "../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<AudioSource> AudioSource::Create()
    {
        AudioApi api = AudioDevice::GetApi();

        DEBUG_ASSERT(api == AudioApi::OpenAL);

        switch(api)
        {
        case AudioApi::OpenAL:
            return std::make_shared<OpenALAudioSource>();
        }

        return nullptr;
    }

    AudioSource::~AudioSource() = default;
}
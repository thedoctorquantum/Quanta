#include <Quanta/Audio/AudioBuffer.h>
#include <Quanta/Audio/AudioDevice.h>
#include <sndfile.h>

#include "../Platform/OpenAL/AudioBuffer.h"
#include "../Debugging/Validation.h"

namespace Quanta
{
    std::shared_ptr<AudioBuffer> AudioBuffer::Create() 
    {
        const AudioApi api = AudioDevice::GetApi();

        DEBUG_ASSERT(api == AudioApi::OpenAL);

        switch(api)
        {
        case AudioApi::OpenAL:
            return std::make_shared<OpenAL::AudioBuffer>();
        }

        return nullptr;
    }
    
    std::shared_ptr<AudioBuffer> AudioBuffer::FromFile(const std::string& filepath)
    {
        SF_INFO sfinfo;

        SNDFILE* const sndfile = sf_open(filepath.c_str(), SFM_READ, &sfinfo);

        DEBUG_ASSERT(sndfile != nullptr);

        SoundFormat format;

        switch(sfinfo.channels)
        {
        case 1:
            format = SoundFormat::Mono16;

            break;
        case 2:
            format = SoundFormat::Stereo16;

            break;
        case 3: 
            if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
            {
                format = SoundFormat::TwoDimentional16;
            }

            break;
        case 4:
            if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
            {
                format = SoundFormat::ThreeDimentional16;
            }

            break;
        }

        const std::unique_ptr<int16_t[]> buffer = std::make_unique<int16_t[]>(sfinfo.frames * sfinfo.channels);

        const sf_count_t frameCount = sf_readf_short(sndfile, buffer.get(), sfinfo.frames);

        DEBUG_ASSERT(frameCount != 0);

        const size_t bufferSize = frameCount * sfinfo.channels * sizeof(short);

        const std::shared_ptr<AudioBuffer> audioBuffer = Create();

        audioBuffer->SetData(buffer.get(), bufferSize, sfinfo.samplerate, format);

        sf_close(sndfile);

        return audioBuffer;
    }
}
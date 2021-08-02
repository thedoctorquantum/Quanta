#include <Quanta/Audio/AudioBuffer.h>
#include <Quanta/Audio/AudioDevice.h>
#include <sndfile.h>

#include "../Platform/Audio/OpenAL/OpenALAudioBuffer.h"

namespace Quanta
{
    std::shared_ptr<AudioBuffer> AudioBuffer::Create() 
    {
        switch(AudioDevice::GetApi())
        {
        case AudioApi::OpenAL:
            return std::make_shared<OpenALAudioBuffer>();
        }

        return nullptr;
    }
    
    std::shared_ptr<AudioBuffer> AudioBuffer::FromFile(const std::string& filepath)
    {
        SNDFILE* sndfile = nullptr;
        SF_INFO sfinfo;
        int16_t* buffer = nullptr;
        size_t bufferSize = 0;
        sf_count_t frameCount = 0;

        sndfile = sf_open(filepath.c_str(), SFM_READ, &sfinfo);

        if(!sndfile)
        {
            fprintf(stderr, "Could not open audio in %s: %s\n", filepath.c_str(), sf_strerror(sndfile));

            return 0;
        }

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

        buffer = (int16_t*) malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(int16_t));

        frameCount = sf_readf_short(sndfile, buffer, sfinfo.frames);

        if(!frameCount)
        {
            free(buffer);

            sf_close(sndfile);
            
            return 0;
        }

        bufferSize = (size_t)(frameCount * sfinfo.channels) * (size_t) sizeof(short);

        std::shared_ptr<AudioBuffer> audioBuffer = Create();

        audioBuffer->SetData(buffer, bufferSize, sfinfo.samplerate, format);

        free(buffer);
        sf_close(sndfile);

        return audioBuffer;
    }
    
    AudioBuffer::~AudioBuffer() = default;
}
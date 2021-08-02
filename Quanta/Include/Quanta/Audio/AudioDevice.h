#pragma once

#include "AudioApi.h"
#include "AudioSource.h"
#include "AudioBuffer.h"

namespace Quanta
{
    class AudioDevice
    {
    public:
        static void Initialize(AudioApi api);
        static void DeInitialize(); 
        
        static AudioApi GetApi();

        static void Play(const AudioSource& source, const AudioBuffer& buffer);
    private:
        virtual void InternalPlay(const AudioSource& source, const AudioBuffer& buffer) = 0;
    };
}
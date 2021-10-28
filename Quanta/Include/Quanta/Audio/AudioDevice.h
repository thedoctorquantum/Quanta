#pragma once

#include "AudioApi.h"
#include "AudioSource.h"
#include "AudioBuffer.h"

namespace Quanta
{
    struct AudioDevice
    {
        static void Create(AudioApi api);
        static void Destroy(); 
        
        static AudioApi GetApi();

        static void Play(const AudioSource& source, const AudioBuffer& buffer);
    private:
        virtual void InternalPlay(const AudioSource& source, const AudioBuffer& buffer) = 0;
    };
}
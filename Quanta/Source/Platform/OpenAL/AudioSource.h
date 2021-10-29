#pragma once

#include <Quanta/Audio/AudioSource.h>
#include <cstdint>

namespace Quanta::OpenAL
{
    struct AudioSource : public Quanta::AudioSource
    {
        AudioSource();
        ~AudioSource();

        AudioSource(const AudioSource&) = delete;

        AudioSource& operator=(const AudioSource&) = delete;

        const glm::vec3& GetTranslation() const override;
        void SetTranslation(const glm::vec3&) override;

        std::uint32_t GetHandle() const;
    private:
        glm::vec3 translation = glm::vec3(0.0f);
        
        std::uint32_t handle = 0;
    };
}
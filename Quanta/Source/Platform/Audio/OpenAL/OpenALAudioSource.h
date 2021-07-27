#pragma once

#include <stdint.h>

#include "../../../Quanta/Audio/AudioSource.h"

namespace Quanta
{
    class OpenALAudioSource final : public AudioSource
    {
    public:
        OpenALAudioSource();
        ~OpenALAudioSource();

        OpenALAudioSource(const OpenALAudioSource&) = delete;

        OpenALAudioSource& operator=(const OpenALAudioSource&) = delete;

        const glm::vec3& GetTranslation() const override;
        void SetTranslation(const glm::vec3& value) override;

        uint32_t GetHandle() const;
    private:
        glm::vec3 translation = glm::vec3(0.0f);
        uint32_t handle = 0;
    };
}
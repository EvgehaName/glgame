#pragma once

#include <AL/al.h>
#include <vector>

class AudioSound
{
    public:
        AudioSound(const std::vector<char>& pcmData, ALenum format, ALsizei sampleRate);
        ~AudioSound();

        void play();
        void stop();

    private:
        ALuint alBuffer;
        ALuint alSource;
};
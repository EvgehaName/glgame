#pragma once

#include <AL/al.h>
#include <vector>

class AudioSound
{
    public:
        AudioSound();
        ~AudioSound();

        void play(const std::vector<char>& pcmData, ALenum format, ALsizei sampleRate);
        void stop();

    private:
        ALuint alBuffer;
        ALuint alSource;
};
#pragma once

#include <AL/alc.h>
#include <iostream>
class AudioContext
{
    public:
        AudioContext();
        ~AudioContext();

    private:
        bool initOpenAL();
        bool deinitOpenAL();

        ALCdevice* alDevice = nullptr;
        ALCcontext* alContext = nullptr;
};
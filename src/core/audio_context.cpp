#include "audio_context.h"

AudioContext::AudioContext()
{
    initOpenAL();
}

AudioContext::~AudioContext()
{
    deinitOpenAL();
}

bool AudioContext::initOpenAL()
{
    alDevice = alcOpenDevice(nullptr);
    if(!alDevice)
    {
        std::cout << "Could not find OpenAL device!" << std::endl;
        return false;
    }

    alContext = alcCreateContext(alDevice, nullptr);
    if(!alContext)
    {
        std::cout << "Could not create OpenAL context!" << std::endl;
        return false;
    }

    if(!alcMakeContextCurrent(alContext))
    {
        std::cout << "Unable to make OpenAL context current!" << std::endl;
        return false;
    }
    return true;
}

bool AudioContext::deinitOpenAL()
{
    return false;
}

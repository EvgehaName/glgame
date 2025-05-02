#include "audio_sound.h"

AudioSound::AudioSound(const std::vector<char>& pcmData, ALenum format, ALsizei sampleRate) {
    alGenBuffers(1, &alBuffer);
    alGenSources(1, &alSource);
    alBufferData(alBuffer, format, pcmData.data(), pcmData.size(), sampleRate);
    alSourcei(alSource, AL_BUFFER, alBuffer);
}

AudioSound::~AudioSound() {
    alDeleteSources(1, &alSource);
    alDeleteBuffers(1, &alBuffer);
}

void AudioSound::play(bool isLoopingSound) {
    alSourcei(alSource, AL_LOOPING, isLoopingSound);
    alSourcePlay(alSource);
}

void AudioSound::stop() {
    alSourceStop(alSource);
}

bool AudioSound::isPlaying()
{
    ALint sourceState;
    alGetSourcei(alSource, AL_SOURCE_STATE, &sourceState);
    return AL_PLAYING == sourceState;
}

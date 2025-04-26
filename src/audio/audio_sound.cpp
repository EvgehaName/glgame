#include "audio_sound.h"

AudioSound::AudioSound() {
    alGenBuffers(1, &alBuffer);
    alGenSources(1, &alSource);
}

AudioSound::~AudioSound() {
    alDeleteSources(1, &alSource);
    alDeleteBuffers(1, &alBuffer);
}

void AudioSound::play(const std::vector<char>& pcmData, ALenum format, ALsizei sampleRate) {
    alBufferData(alBuffer, format, pcmData.data(), pcmData.size(), sampleRate);
    alSourcei(alSource, AL_BUFFER, alBuffer);
    alSourcePlay(alSource);
}

void AudioSound::stop() {
    alSourceStop(alSource);
}

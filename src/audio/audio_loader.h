#pragma once

#include <string>
#include <vector>
#include <vorbis/vorbisfile.h>
#include <AL/al.h>
#include <fstream>
#include <iostream>

class AudioLoader
{
    public:
        AudioLoader(const std::string& filepath);
        ~AudioLoader();

        const std::vector<char>& getPCM() const { return pcm; }
        int getChannels() const { return channels; }
        int getSampleRate() const { return sampleRate; }
        ALenum getFormat() const { return format; }
    private:
        std::vector<char> pcm;
        int channels;
        int sampleRate;
        ALenum format;
        OggVorbis_File vorbisFile;
        vorbis_info* vorbisInfo;
        FILE* oggFile;
};
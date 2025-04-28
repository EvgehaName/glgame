#include "audio_loader.h"

AudioLoader::AudioLoader(const std::string &filepath)
{
    oggFile = fopen(filepath.c_str(), "rb");
    if(!oggFile)
    {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    if(ov_open_callbacks(oggFile, &vorbisFile, nullptr, 0, OV_CALLBACKS_NOCLOSE) < 0)
    {
        throw std::runtime_error("Failed to open OGG stream: " + filepath);
    }
    vorbisInfo = ov_info(&vorbisFile, -1);

    channels = vorbisInfo->channels;
    sampleRate = vorbisInfo->rate;
    format = (vorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    constexpr size_t bufferSize = 4096;
    char buffer[bufferSize];
    int bitstream;
    long bytesRead;
    
    while ((bytesRead = ov_read(&vorbisFile, buffer, bufferSize, 0, 2, 1, &bitstream)) > 0) {
        pcm.insert(pcm.end(), buffer, buffer + bytesRead);
    }
}

AudioLoader::~AudioLoader()
{
    ov_clear(&vorbisFile);
    fclose(oggFile);
}

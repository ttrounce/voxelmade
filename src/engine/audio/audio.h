#ifndef VHM_AUDIO_H
#define VHM_AUDIO_H

#include "audioformat.h"
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <al/alc.h>
#include <al/al.h>
// #include <iostream>

using alcdevice_ptr = std::unique_ptr<ALCdevice, decltype(&alcCloseDevice)>;
using alccontext_ptr = std::unique_ptr<ALCcontext, decltype(&alcDestroyContext)>;

namespace vhm
{

void GetAvailableAudioDevices(std::vector<std::string>& devices);
alcdevice_ptr GetDefaultAudioDevice();
alccontext_ptr GetAudioContext(ALCdevice* device);

struct AUDIO_SAMPLE
{
    ALuint buffer;
    AUDIO_SAMPLE(){}
    ~AUDIO_SAMPLE()
    {
        alDeleteBuffers(1, &buffer);
    }
};

class AUDIO_MANAGER
{
private:
    alccontext_ptr context;
    alcdevice_ptr mainDevice;
    std::map<std::string, std::shared_ptr<AUDIO_SAMPLE>> samples;

public:
    AUDIO_MANAGER();
    ~AUDIO_MANAGER();

    void LoadSampleWAV(std::string name, WAV_DATA& wav);

    // TODO: create some greater controls over the sounds, how they're played i.e. looping, volume, etc

    uint PlaySampleOnce(std::string name, ALfloat volume, ALfloat pitch);
    uint PlaySampleLooped(std::string name, ALfloat volume, ALfloat pitch, bool (*shouldLoop)(ALuint source));
};

extern std::unique_ptr<AUDIO_MANAGER> audioManager;

}

#endif // VHM_AUDIO_H
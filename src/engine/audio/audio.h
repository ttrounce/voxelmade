#ifndef VHM_AUDIO_H
#define VHM_AUDIO_H

#include "../utility/types.h"
#include "../utility/logging.h"
#include "audioformat.h"
#include <map>
#include <vector>
#include <string>
#include <al/alc.h>
#include <al/al.h>
#include <iostream>
#include <thread>

namespace vhm
{

u32 FreeAudioDevice(ALCdevice* device);
void FreeAudioContext(ALCcontext* context);
void GetAvailableAudioDevices(std::vector<std::string>& devices);
u32 GetDefaultAudioDevice(ALCdevice*& device);
u32 GetAudioContext(ALCcontext*& context, ALCdevice* device);

struct AUDIO_SAMPLE
{
    ALuint buffer;
};

class AUDIO_MANAGER
{
private:
    ALCcontext* context;
    ALCdevice* mainDevice;
    std::map<std::string, AUDIO_SAMPLE*> samples;

    AUDIO_MANAGER();
    ~AUDIO_MANAGER();
public:
    static AUDIO_MANAGER* GetInstance()
    {
        static AUDIO_MANAGER manager;
        return &manager;
    }

    void LoadSampleWAV(std::string name, WAV_DATA& wav);

    // TODO: create some greater controls over the sounds, how they're played i.e. looping, volume, etc

    u32 PlaySampleOnce(std::string name, ALfloat volume, ALfloat pitch);
    u32 PlaySampleLooped(std::string name, ALfloat volume, ALfloat pitch, bool (*shouldLoop)(ALuint source));
};

}

#endif // VHM_AUDIO_H
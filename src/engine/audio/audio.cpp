#include "audio.h"

using namespace vhm;

u32 vhm::FreeAudioDevice(ALCdevice* device)
{
    if(alcCloseDevice(device))
    {
        return VHM_SUCCESS;
    }
    return VHM_ERROR;   
}

void vhm::FreeAudioContext(ALCcontext* context)
{
    alcDestroyContext(context);
}

void vhm::GetAvailableAudioDevices(std::vector<std::string>& devices)
{
    ALCdevice* device = NULL;
    const ALCchar* str = alcGetString(device, ALC_DEVICE_SPECIFIER);
    const ALCchar* ptr = str;
    do
    {
        devices.push_back(std::string(ptr));
        ptr += devices.back().size() + 1;
    } while(*(ptr + 1) != '\0');
}

u32 vhm::GetDefaultAudioDevice(ALCdevice*& device)
{
    device = alcOpenDevice(nullptr);
    if(device && alcGetError(device) == ALC_NO_ERROR)
    {
        return VHM_SUCCESS;
    }
    return VHM_ERROR;
}

u32 vhm::GetAudioContext(ALCcontext*& context, ALCdevice* device)
{
    context = alcCreateContext(device, NULL);
    if(context && alcGetError(device) == ALC_NO_ERROR)
    {
        return VHM_SUCCESS;
    }
    return VHM_ERROR;
}

struct AUDIO_SAMPLE
{
    ALuint buffer;
};

AUDIO_MANAGER::AUDIO_MANAGER()
{
    GetDefaultAudioDevice(mainDevice);
    GetAudioContext(context, mainDevice);
    alcMakeContextCurrent(context);
}
AUDIO_MANAGER::~AUDIO_MANAGER()
{
    for(auto it = samples.begin(); it != samples.end(); it++)
    {
        alDeleteBuffers(1, &it->second->buffer);
        delete &it->second;
    }

    FreeAudioContext(context);
    FreeAudioDevice(mainDevice);
}
void AUDIO_MANAGER::LoadSampleWAV(std::string name, WAV_DATA& wav)
{

    AUDIO_SAMPLE* sample = new AUDIO_SAMPLE;
    alGenBuffers(1, &sample->buffer);
    alBufferData(sample->buffer, wav.format, wav.data, wav.dataSize, wav.sampleRate);
    samples.insert(std::make_pair(name, sample));
}

// TODO: create some greater controls over the sounds, how they're played i.e. looping, volume, etc

u32 AUDIO_MANAGER::PlaySampleOnce(std::string name, ALfloat volume, ALfloat pitch)
{
    if(samples.count(name) == 0)
    {
        printf("%s No sample found with name %s", VHM_ENGINE_ERR, name.c_str());
        return VHM_ERROR;
    }
    AUDIO_SAMPLE* sample = samples.at(name);

    // Going to stick with a new thread, might make some sort of Thread Pool later.
    std::thread([volume, pitch](ALuint buffer){
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);            
        alSourcef(source, AL_GAIN, volume);
        alSourcef(source, AL_PITCH, pitch);
        alSourcePlay(source);
        ALint state = AL_PLAYING;
        while(state == AL_PLAYING)
        {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        }
        alDeleteSources(1, &source);
    }, sample->buffer).detach();

    return VHM_SUCCESS;
}

u32 AUDIO_MANAGER::PlaySampleLooped(std::string name, ALfloat volume, ALfloat pitch, bool (*shouldLoop)(ALuint source))
{
    if(samples.count(name) == 0)
    {
        printf("%s No sample found with name %s", VHM_ENGINE_ERR, name.c_str());
        return VHM_ERROR;
    }
    AUDIO_SAMPLE* sample = samples.at(name);

    // Going to stick with a new thread, might make some sort of Thread Pool later.
    std::thread([shouldLoop, volume, pitch](ALuint buffer){
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcef(source, AL_GAIN, volume);
        alSourcef(source, AL_PITCH, pitch);
        alSourcePlay(source);
        ALint state = AL_PLAYING;
        while(state == AL_PLAYING)
        {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            alSourcei(source, AL_LOOPING, shouldLoop(source));
        }
        alDeleteSources(1, &source);
    }, sample->buffer).detach();


    return VHM_SUCCESS;
}
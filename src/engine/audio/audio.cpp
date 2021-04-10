#include "../utility/logging.h"
#include "audio.h"
#include <thread>

using namespace vhm;

std::unique_ptr<AUDIO_MANAGER> vhm::audioManager = std::make_unique<AUDIO_MANAGER>();

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

alcdevice_ptr vhm::GetDefaultAudioDevice()
{
    auto device = alcOpenDevice(nullptr);
    if(device && alcGetError(device) == ALC_NO_ERROR)
    {
        return alcdevice_ptr(device, alcCloseDevice);
    }
    return alcdevice_ptr(nullptr, nullptr);
}

alccontext_ptr vhm::GetAudioContext(ALCdevice* device)
{
    auto context = alcCreateContext(device, NULL);
    if(context && alcGetError(device) == ALC_NO_ERROR)
    {
        return alccontext_ptr(context, alcDestroyContext);
    }
    return alccontext_ptr(nullptr, nullptr);
}

AUDIO_MANAGER::AUDIO_MANAGER() : context(nullptr, nullptr), mainDevice(nullptr, nullptr)
{
    mainDevice = GetDefaultAudioDevice();
    context = GetAudioContext(mainDevice.get());
    alcMakeContextCurrent(context.get());
}
AUDIO_MANAGER::~AUDIO_MANAGER()
{
    samples.clear();
}

void AUDIO_MANAGER::LoadSampleWAV(std::string name, WAV_DATA& wav)
{

    std::shared_ptr<AUDIO_SAMPLE> sample = std::make_shared<AUDIO_SAMPLE>();
    alGenBuffers(1, &sample->buffer);
    alBufferData(sample->buffer, wav.format, wav.data, wav.dataSize, wav.sampleRate);
    samples.insert(std::make_pair(name, sample));
}

// TODO: create some greater controls over the sounds, how they're played i.e. looping, volume, etc

uint AUDIO_MANAGER::PlaySampleOnce(std::string name, ALfloat volume, ALfloat pitch)
{
    if(samples.count(name) == 0)
    {
        printf("%s No sample found with name %s", VHM_ENGINE_ERR, name.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<AUDIO_SAMPLE> sample = samples.at(name);

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

uint AUDIO_MANAGER::PlaySampleLooped(std::string name, ALfloat volume, ALfloat pitch, bool (*shouldLoop)(ALuint source))
{
    if(samples.count(name) == 0)
    {
        printf("%s No sample found with name %s", VHM_ENGINE_ERR, name.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<AUDIO_SAMPLE> sample = samples.at(name);

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
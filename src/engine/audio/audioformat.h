#ifndef VHM_AUDIO_FILES_H
#define VHM_AUDIO_FILES_H

#include "../utility/types.h"
#include "../utility/logging.h"
#include <stdio.h>
#include <string>

#include <AL/al.h>

namespace vhm
{

struct WAV_DATA
{
    int channels;
    int sampleRate;
    int bitsPerSample;
    int format;
    int dataSize;
    uchar* data;
};

size_t ReadStringBuffer(FILE* file, uint size, char* buffer);
size_t ReadintBuffer(FILE* file, uint size, int* buffer);

void FreeWAV(WAV_DATA& waveData);

uint LoadWAV(std::string path, WAV_DATA& waveData);

}

#endif // VHM_AUDIO_FILES_H
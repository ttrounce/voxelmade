#ifndef VHM_AUDIO_FILES_H
#define VHM_AUDIO_FILES_H

#include "../utility/types.h"
#include "../utility/logging.h"
#include <stdio.h>
#include <string.h>

#include <AL/al.h>

namespace vhm
{

struct WAV_DATA
{
    i32 channels;
    i32 sampleRate;
    i32 bitsPerSample;
    i32 format;
    i32 dataSize;
    u8* data;
};

size_t ReadStringBuffer(FILE* file, u32 size, char* buffer);
size_t ReadI32Buffer(FILE* file, u32 size, i32* buffer);

void FreeWAV(WAV_DATA& waveData);

u32 LoadWAV(const char* path, WAV_DATA& waveData);

}

#endif // VHM_AUDIO_FILES_H
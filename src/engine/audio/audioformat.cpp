#include "audioformat.h"
#include <string.h>

using namespace vhm;

size_t vhm::ReadStringBuffer(FILE* file, uint size, char* buffer)
{
    return fread(buffer, 1, size, file);
}

size_t vhm::ReadintBuffer(FILE* file, uint size, int* buffer)
{
    *buffer = 0;
    return fread(buffer, 1, size, file);
}

void vhm::FreeWAV(WAV_DATA& waveData)
{
    delete[] waveData.data;
}

uint vhm::LoadWAV(std::string path, WAV_DATA& waveData)
{
    FILE* file = fopen(path.c_str(), "rb");
    if(file)
    {
        fseek(file, 0, SEEK_SET);

        char buffer[4];
        int intbuf = 0;

        int channels = 0;
        int sampleRate = 0;
        int bitsPerSample = 0;
        int dataSize =0;

        // RIFF Check
        if(!ReadStringBuffer(file, 4, buffer) || strncmp(buffer, "RIFF", 4) != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        
        // File Size
        if(!fread(buffer, 1, 4, file))
        {
            fclose(file);
            return VHM_ERROR;
        }
        
        // WAVE Check
        if(!ReadStringBuffer(file, 4, buffer) || strncmp(buffer, "WAVE", 4) != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // FMT Check
        if(!ReadStringBuffer(file, 4, buffer) || strncmp(buffer, "fmt ", 4) != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Length of format data
        if(!ReadintBuffer(file, 4, &intbuf) || intbuf != 16)
        {  
            fclose(file);
            return VHM_ERROR;
        }

        // Type of format (Always 1)
        if(!ReadintBuffer(file, 2, &intbuf) || intbuf != 1)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Number of channels
        if(!ReadintBuffer(file, 2, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        channels = intbuf;
        intbuf = 0;

        // Sample Rate
        if(!ReadintBuffer(file, 4, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        sampleRate = intbuf;
        intbuf = 0;

        // Sample Rate * BitsPerSample * Channels / 8
        if(!ReadintBuffer(file, 4, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Bits Per Channel Sample
        if(!ReadintBuffer(file, 2, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        int fmt = intbuf;
        intbuf = 0;

        // Bits per Sample
        if(!ReadintBuffer(file, 2, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        bitsPerSample = intbuf;
        intbuf = 0;

        // Data chunk marker
        if(!ReadStringBuffer(file, 4, buffer) || strncmp(buffer, "data", 4) != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Size of data section
        if(!ReadintBuffer(file, 4, &intbuf) || intbuf <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        dataSize = intbuf;

        switch(fmt)
        {
            case 1:
                waveData.format = AL_FORMAT_MONO8;
                break;
            case 2:
                waveData.format = (bitsPerSample  == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO16);
                break;
            case 4:
                waveData.format = AL_FORMAT_STEREO16;
                break;
            default:
                fclose(file);
                return VHM_ERROR;
        }

        waveData.channels = channels;
        waveData.bitsPerSample = bitsPerSample;
        waveData.sampleRate = sampleRate;
        waveData.dataSize = dataSize;
        waveData.data = new uchar[dataSize];

        fread(waveData.data, 1, dataSize, file);

        fclose(file);
        return VHM_SUCCESS;
    }
    else printf("%s File not found %s\n", VHM_ENGINE_ERR, path.c_str());
    return VHM_ERROR;
}
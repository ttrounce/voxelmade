#include "../utility/io.h"
#include "audioformat.h"
#include <string.h>

using namespace vhm;

std::string ReadString(FILE* file, uint size)
{
    char buf[size];
    if(fread(buf, 1, size, file) == size)
        return std::string(buf);
    return std::string();
}

uint32_t ReadInt(FILE* file, uint size)
{
    uint32_t buf = 0;
    if(fread(&buf, 1, size, file) == size)
        return buf;
    return 0;
}

uint64_t ReadLong(FILE* file, uint size)
{
    uint64_t buf = 0;
    if(fread(&buf, 1, size, file) == size)
        return buf;
    return 0;
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

        int channels = 0;
        int sampleRate = 0;
        int bitsPerSample = 0;
        int dataSize =0;

        // RIFF Check
        std::string riff;
        if((riff = ReadString(file, 4)).empty() || riff.compare("RIFF") != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }
        
        // File Size
        ReadInt(file, 4);
        
        // WAVE Check
        std::string wave;
        if((wave = ReadString(file, 4)).empty() || wave.compare("WAVE") != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // FMT Check
        std::string format;
        if((format = ReadString(file, 4)).empty() || format.compare("fmt ") != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Length of format data
        if(ReadInt(file, 4) <= 0)
        {  
            fclose(file);
            return VHM_ERROR;
        }

        // Type of format (Always 1)
        int type = 0;
        if((type = ReadInt(file, 2)) != 1)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Number of channels

        if((channels = ReadInt(file, 2)) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Sample Rate
        if((sampleRate = ReadInt(file, 4)) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Sample Rate * BitsPerSample * Channels / 8
        if(ReadInt(file, 4) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Bits Per Channel Sample
        int fmt = 0;
        if((fmt = ReadInt(file, 2)) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Bits per Sample
        if((bitsPerSample = ReadInt(file, 2)) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Data chunk marker
        std::string dataMarker;
        if((dataMarker = ReadString(file, 4)).empty() || dataMarker.compare("data") != 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        // Size of data section
        if((dataSize = ReadInt(file, 4)) <= 0)
        {
            fclose(file);
            return VHM_ERROR;
        }

        if(fmt == 1 && bitsPerSample == 8)
            waveData.format = AL_FORMAT_MONO8;
        else if(fmt == 2 && bitsPerSample == 8)
            waveData.format = AL_FORMAT_STEREO8;
        else if(fmt == 2 && bitsPerSample == 16)
            waveData.format = AL_FORMAT_MONO16;
        else if(fmt == 4 && bitsPerSample == 16)
            waveData.format = AL_FORMAT_STEREO16;
        else 
        {
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
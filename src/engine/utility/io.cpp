#include "io.h"

#include "logging.h"
#include "types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

std::string vhm::ReadString(std::string path)
{
    FILE* file = fopen(path.c_str(), "rb");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* source = new char[size + 1];
        if(source)
        {
            fread(source, 1, size, file);
            source[size] = '\0';
        }
        fclose(file);
        std::string outputSource(source);
        delete[] source;
        return outputSource;
    }
    else printf("%s File not found %s\n", VHM_ENGINE_ERR, path.c_str());
    return NULL;
}

void vhm::WriteString(std::string path, std::string value, int length)
{
    FILE* file = fopen(path.c_str(), "wb");
    if(file)
    {
        fwrite(value.c_str(), sizeof(char), length, file);
        fclose(file);
    }
    else printf("%s File not found %s\n", VHM_ENGINE_ERR, path.c_str());
}

uint32_t vhm::BytesToU32(uchar* bytes, int count)
{
    count = (count > 4 ? 4 : count);

    uint32_t final = 0;
    uchar* organisedBytes = new uchar[count];
    for(int i = 0; i < count; i++)
    {
        memcpy(&organisedBytes[count - i - 1], &bytes[i], 1);
    }
    memcpy(&final, &organisedBytes[0], count);
    delete[] organisedBytes;
    return final;
}

uint64_t vhm::BytesToU64(uchar* bytes, int count)
{
    count = (count > 8 ? 8 : count);

    uint64_t final = 0;
    uchar* organisedBytes = new uchar[count];
    for(int i = 0; i < count; i++)
    {
        memcpy(&organisedBytes[count - i - 1], &bytes[i], 1);
    }
    memcpy(&final, &organisedBytes[0], count);
    delete[] organisedBytes;
    return final;
}

uint64_t vhm::ReadBits(FILE*& file, int bits, int& bitOffset)
{
    if(bits <= 0)
        return -1;
    if(bitOffset < 0 || bitOffset >= 8)
        return -1;

    // The span of bytes we need to read the required bits
    int searchBitCount = bits + bitOffset;
    int byteCount = (int) floor(searchBitCount / 8.0);
    int bitRemainderCount = searchBitCount - 8*byteCount;

    // The total amount of bytes to read
    int readCount = byteCount + (bitRemainderCount > 0 ? 1 : 0);
    if(readCount <= 0)
    {
        return -1;
    }
    uchar uncutBytes[readCount];
    fread(uncutBytes, 1, readCount, file);
    if(bitRemainderCount > 0)
    {
        // Seek to the previous byte when using a partial byte
        fseek(file, -1L, SEEK_CUR);
    }

    // Bytes concatenated into a 64bit integer
    uint64_t concatenatedBytes = BytesToU64(uncutBytes, readCount);

    // Form the bitmask to remove the offset
    int offsetBitmask = ~0;
    if(bitOffset > 0)
    {
        offsetBitmask = ~(((1 << bitOffset) - 1) << (readCount * 8 - bitOffset));
    }

    // Form the bitmask to remove the trailing bits
    int clip = (byteCount + (bitRemainderCount > 0 ? 1 : 0)) * 8 - bitOffset - bits;
    int clipBitmask = ~0;
    if(clip > 0)
    {
        clipBitmask = ~((1 << clip) - 1);
    }

    // Merge the bitmasks together and apply them to the unsigned 64bit integer
    int finalBitmask = offsetBitmask & clipBitmask;

    uint64_t finalBits = (concatenatedBytes & finalBitmask) >> clip;
    // The bit offset (for the next read operation)
    bitOffset = (8 - clip) % 8;
    return finalBits;
}
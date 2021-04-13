#ifndef VHM_IO_H
#define VHM_IO_H

#include "types.h"
#include <string>

namespace vhm
{

std::string ReadString(std::string path);
void        WriteString(std::string path, std::string value, int length);

uint32_t    BytesToU32(uchar* bytes, int count);
uint64_t    BytesToU64(uchar* bytes, int count);
uint64_t    ReadBits(FILE*& file, int bits, int& bitOffset);

}
#endif // VHM_IO_H
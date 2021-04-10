#ifndef VHM_IO_H
#define VHM_IO_H

#include <string>

namespace vhm
{

std::string ReadString(std::string path);
void WriteString(std::string path, std::string value, int length);

}
#endif // VHM_IO_H
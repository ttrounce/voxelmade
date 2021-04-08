#ifndef VHM_IO_H
#define VHM_IO_H

namespace vhm
{

char* ReadString(const char* path);
void WriteString(const char* path, const char* value, int length);

}
#endif // VHM_IO_H
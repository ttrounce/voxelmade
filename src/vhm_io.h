#ifndef VHM_IO_H
#define VHM_IO_H

char* vhm_ReadString(const char* path);
void vhm_WriteString(const char* path, const char* value, int length);

#endif // VHM_IO_H
#include "basicio.h"

#include "logging.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>

char* vhm::ReadString(const char* path)
{
    FILE* file = fopen(path, "rb");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        i32 size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* source = (char*) malloc(size + 1);
        if(source)
        {
            fread(source, 1, size, file);
            source[size] = '\0';
        }
        fclose(file);
        return source;
    }
    else printf("%s File not found %s\n", VHM_ENGINE_ERR, path);
    return NULL;
}

void vhm::WriteString(const char* path, const char* value, int length)
{
    FILE* file = fopen(path, "wb");
    if(file)
    {
        fwrite(value, sizeof(char), length, file);
        fclose(file);
    }
    else printf("%s File not found %s\n", VHM_ENGINE_ERR, path);
}
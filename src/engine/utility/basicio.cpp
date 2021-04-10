#include "basicio.h"

#include "logging.h"
#include "types.h"
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
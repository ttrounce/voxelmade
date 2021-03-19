#include <gtest/gtest.h>
#include "vhm_io.h"

#include <stdio.h>

void build_empty_file(const char* path)
{   
    FILE* file = fopen(path, "wb");
    if(file)
    {
        fclose(file);
    }
}

void build_content_file(const char* path, const char* value, int length)
{
    FILE* file = fopen(path, "wb");
    if(file)
    {
        fwrite(value, sizeof(char), length, file);
        fclose(file);
    }
}

const char* file_empty_path = "empty_file_test.txt";
const char* file_filled_path = "filled_file_test.txt";

TEST(VHM_IO_TEST, READ_EMPTY_FILE) {
    build_empty_file(file_empty_path);
    ASSERT_STREQ(vhm_ReadString(file_empty_path), "");
    remove(file_empty_path);
}

TEST(VHM_IO_TEST, WRITE_AND_READ_FILLED_FILE) {
    const char* text = "this is test text";
    int length = strlen(text);
    vhm_WriteString(file_filled_path, text, length);
    ASSERT_STREQ(vhm_ReadString(file_filled_path), text);
    remove(file_filled_path);
}
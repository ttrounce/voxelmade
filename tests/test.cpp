#include "engine/utility/basicio.h"
#include "engine/utility/glm.h"

#include <gtest/gtest.h>
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
    ASSERT_STREQ(vhm::ReadString(file_empty_path).c_str(), "");
    remove(file_empty_path);
}

TEST(VHM_IO_TEST, WRITE_AND_READ_FILLED_FILE) {
    const char* text = "this is test text";
    int length = strlen(text);
    vhm::WriteString(file_filled_path, text, length);
    ASSERT_STREQ(vhm::ReadString(file_filled_path).c_str(), text);
    remove(file_filled_path);
}
/* ------------- VHM_GLM ------------- */


TEST(VHM_GLM_TEST, VEC2_SET) {
    vec2 result;
    vhm::glm_Vec2Set(5.23, -323.4, result);

    ASSERT_FLOAT_EQ(result[0], 5.23);
    ASSERT_FLOAT_EQ(result[1], -323.4);
}

TEST(VHM_GLM_TEST, VEC3_SET) {
    vec3 result;
    vhm::glm_Vec3Set(5.23, -323.4, 0.00034, result);

    ASSERT_FLOAT_EQ(result[0], 5.23);
    ASSERT_FLOAT_EQ(result[1], -323.4);
    ASSERT_FLOAT_EQ(result[2], 0.00034);
}

TEST(VHM_GLM_TEST, VEC4_SET) {
    vec4 result;
    vhm::glm_Vec4Set(5.23, -323.4, 0.00034, -393550.05, result);

    ASSERT_FLOAT_EQ(result[0], 5.23);
    ASSERT_FLOAT_EQ(result[1], -323.4);
    ASSERT_FLOAT_EQ(result[2], 0.00034);
    ASSERT_FLOAT_EQ(result[3], -393550.05);
}
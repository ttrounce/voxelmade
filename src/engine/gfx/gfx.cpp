#include "../utility/logging.h"
#include "../utility/types.h"
#include "gfx.h"

#include <stb_image.h>
#include <stdio.h>

#define MAX_VBOS 16

using namespace vhm;

void vhm::SetBufferVAO(vhm::VAO& vao, GLuint index, GLint size, void* data, GLenum type, GLint length)
{
    if(index < MAX_VBOS)
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
        switch(type)
        {
            case GL_FLOAT:
            case GL_DOUBLE:
                glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, 0);
                break;
            default:
                glVertexAttribIPointer(index, size, GL_FLOAT, 0, 0);
        }
        glEnableVertexAttribArray(index);
        vao.vbos.push_back(vbo);
    }
    else printf("%s Only VBO indices between 0 and %d can be used\n", VHM_ENGINE_ERR, MAX_VBOS);
}

// Shader Program

void vhm::CreateVertexShader(GLuint program, std::string source, GLint sourceLength)
{
    const char* trueSource = source.c_str();
    GLuint handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(handle, 1, &trueSource, &sourceLength);
    glCompileShader(handle);

    GLint success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        GLchar log[logSize];
        glGetShaderInfoLog(handle, logSize, &logSize, log);

        printf("%s Vertex shader could not compile: \n", VHM_GL_ERR);
        printf("    %s\n", log);

        glDeleteShader(handle);
        return;
    }
    glAttachShader(program, handle);
    glDeleteShader(handle);
}

void vhm::CreateFragmentShader(GLuint program, std::string source, GLint sourceLength)
{
    const char* trueSource = source.c_str();
    GLuint handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handle, 1, &trueSource, &sourceLength);
    glCompileShader(handle);

    GLint success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        GLchar log[logSize];
        glGetShaderInfoLog(handle, logSize, &logSize, log);

        printf("%s Fragment shader could not compile: \n", VHM_GL_ERR);
        printf("    %s\n", log);

        glDeleteShader(handle);
        return;
    }
    glAttachShader(program, handle);
    // screw giving the user the handle
    glDeleteShader(handle);
}

void vhm::LinkProgram(GLuint program)
{
    glLinkProgram(program);
    glValidateProgram(program);
    
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        GLchar log[logSize];
        glGetProgramInfoLog(program, logSize, &logSize, log);

        printf("%s Program could not link: \n", VHM_GL_ERR);
        printf("    %s\n", log);
        glDeleteProgram(program);
    }
}

void vhm::UniformVec2(GLuint program, std::string name, glm::vec2 vec)
{
    glUniform2f(glGetUniformLocation(program, name.c_str()), vec[0], vec[1]);
}

void vhm::UniformVec3(GLuint program, std::string name, glm::vec3 vec)
{
    glUniform3f(glGetUniformLocation(program, name.c_str()), vec[0], vec[1], vec[2]);
}

void vhm::UniformVec4(GLuint program, std::string name, glm::vec4 vec)
{
    glUniform4f(glGetUniformLocation(program, name.c_str()), vec[0], vec[1], vec[2], vec[3]);
}

void vhm::UniformMat2(GLuint program, std::string name, glm::mat2 value)
{
    glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, false, &value[0][0]);
}

void vhm::UniformMat3(GLuint program, std::string name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, false, &value[0][0]);
}

void vhm::UniformMat4(GLuint program, std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, false, &value[0][0]);
}

// Texture

void vhm::SetMinMag(GLenum target, GLint min, GLint mag)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}

void vhm::LoadTexture(GLuint handle, void* data, GLsizei width, GLsizei height, GLint internalFormat, GLenum format)
{
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void vhm::LoadTextureArray(GLuint handle, void* data, GLsizei width, GLsizei height, GLsizei depth, GLint internalFormat, GLenum format)
{
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);
}

/**
 * @param internalFormat see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage3D.xhtml for valid formats.
 */
void vhm::SetTextureArrayStorage(GLuint handle, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, width, height, depth);
}

void vhm::LoadTextureArrayLayer(GLuint handle, void* data, GLint x, GLint y, GLsizei width, GLsizei height, GLint layer, GLenum format)
{
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, x, y, layer, width, height, 1, format, GL_UNSIGNED_BYTE, data);
}

uint vhm::CreateQuickTexture(GLuint target, GLuint* textureHandle, std::string path, GLint min, GLint mag)
{
    GLint w, h, c;
    uchar* data = stbi_load(path.c_str(), &w, &h, &c, STBI_rgb_alpha);

    if(!data)
    {
        printf("%s Unable to load texture due to an unknown error\n", VHM_ENGINE_ERR);
        return VHM_ERROR;
    }

    GLenum format;

    switch(c)
    {
        case STBI_rgb:
            format = GL_RGB;
            break;
        case STBI_rgb_alpha:
            format = GL_RGBA;
            break;
        default:
            printf("%s Unable to load texture due to an unexpected pixel format\n", VHM_ENGINE_ERR);
            stbi_image_free(data);
            return VHM_ERROR;
    }

    glGenTextures(1, textureHandle);
    glBindTexture(target, *textureHandle);
    SetMinMag(target, min, mag);
    LoadTexture(*textureHandle, (void*) data, w, h, format, format);
    glBindTexture(target, 0);
    stbi_image_free(data);
    return VHM_SUCCESS;
}
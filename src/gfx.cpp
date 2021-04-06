#include "gfx.h"
#include "logging.h"

#include <stdio.h>
#include <glad/glad.h>

#define MAX_VBOS 16

void vhm::SetBufferVAO(vhm::VAO& vao, u32 index, u32 size, void* data, u32 type, u32 length)
{
    if(index < MAX_VBOS)
    {
        u32 vbo;
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
    else printf("%s Only VBO indices between 0 and %d can be used\n", index, MAX_VBOS);
}

// Shader Program

void vhm::CreateVertexShader(u32 program, const char* source, i32 sourceLength)
{
    u32 handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(handle, 1, &source, &sourceLength);
    glCompileShader(handle);

    i32 success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        int logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        char log[logSize];
        glGetShaderInfoLog(handle, logSize, &logSize, log);

        printf("%s Vertex shader could not compile: \n", VHM_GL_ERR);
        printf("    %s\n", log);

        glDeleteShader(handle);
        return;
    }
    glAttachShader(program, handle);
    glDeleteShader(handle);
}

void vhm::CreateFragmentShader(u32 program, const char* source, i32 sourceLength)
{
    u32 handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handle, 1, &source, &sourceLength);
    glCompileShader(handle);

    i32 success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        int logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        char log[logSize];
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

void vhm::LinkProgram(u32 program)
{
    glLinkProgram(program);
    glValidateProgram(program);
    
    i32 success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
        int logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        char log[logSize];
        glGetProgramInfoLog(program, logSize, &logSize, log);

        printf("%s Program could not link: \n", VHM_GL_ERR);
        printf("    %s\n", log);
        glDeleteProgram(program);
    }
}

void vhm::UniformVec2(u32 program, const char* name, vec2 vec)
{
    glUniform2f(glGetUniformLocation(program, name), vec[0], vec[1]);
}

void vhm::UniformVec3(u32 program, const char* name, vec3 vec)
{
    glUniform3f(glGetUniformLocation(program, name), vec[0], vec[1], vec[2]);
}

void vhm::UniformVec4(u32 program, const char* name, vec4 vec)
{
    glUniform4f(glGetUniformLocation(program, name), vec[0], vec[1], vec[2], vec[3]);
}

void vhm::UniformMat2(u32 program, const char* name, mat2 value)
{
    glUniformMatrix2fv(glGetUniformLocation(program, name), 1, false, &value[0][0]);
}

void vhm::UniformMat3(u32 program, const char* name, mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, false, &value[0][0]);
}

void vhm::UniformMat4(u32 program, const char* name, mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, false, &value[0][0]);
}

// Texture

void vhm::SetMinMag(u32 target, u32 min, u32 mag)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}

void vhm::LoadTexture(u32 handle, u8* data, u32 width, u32 height, u32 internalFormat, u32 format)
{
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void vhm::LoadTextureArray(u32 handle, u8* data, u32 width, u32 height, u32 depth, u32 internalFormat, u32 format)
{
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);
}

/**
 * @param internalFormat see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage3D.xhtml for valid formats.
 */
void vhm::SetTextureArrayStorage(u32 handle, u32 internalFormat, u32 width, u32 height, u32 depth)
{
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, width, height, depth);
}

void vhm::LoadTextureArrayLayer(u32 handle, u8* data, u32 x, u32 y, u32 width, u32 height, u32 layer, u32 format)
{
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, x, y, layer, width, height, 1, format, GL_UNSIGNED_BYTE, data);
}
#include "vhm_gl.h"
#include <glad/glad.h>
#include <stdio.h>

#define MAX_VBOS 16

void vhm_SetBufferVAO(u32 handle, u32 vbo, u32 index, u32 size, void* data, u32 type, u32 length)
{
    if(index < MAX_VBOS)
    {
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
    }
    else printf("%s Only VBO indices between 0 and %d can be used\n", index, MAX_VBOS);
}

#include <stdio.h>

void vhm_CreateVertexShader(u32 program, const char* source, i32 sourceLength)
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
}

void vhm_CreateFragmentShader(u32 program, const char* source, i32 sourceLength)
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

void vhm_LinkProgram(u32 program, u32* shaders, u32 shaderCount)
{
    glLinkProgram(program);
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
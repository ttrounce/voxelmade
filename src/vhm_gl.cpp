#include "vhm_gl.h"
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
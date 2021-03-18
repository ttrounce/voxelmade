#include "vhm_engine.h"
#include "vhm_gl.h"
#include "vhm_io.h"

#include <glad/glad.h>
#include <stdio.h>
#include <string.h>

u32 vao;
u32 vbo;

u32 program;

void vhm_Init()
{
    char* frag = vhm_ReadString("frag.glsl");

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    f32 data[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 }; 

    glBindVertexArray(vbo);
    vhm_SetBufferVAO(vao, vbo, 0, 2, data, GL_FLOAT, sizeof(data)/sizeof(f32));

    program = glCreateProgram();
    vhm_CreateFragmentShader(program, frag, strlen(frag));
    vhm_LinkProgram(program, 0, 0);
}

void vhm_Clean()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
}

void vhm_Update(f64 time, f64 dt)
{

}

void vhm_Draw()
{
    glUseProgram(program);
    glBindVertexArray(vbo);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
    glUseProgram(0);
}

int main()
{
    if(vhm_InitEngine() == VHM_SUCCESS)
    {
        vhm_Init();
        vhm_StartEngine(vhm_Update, vhm_Draw);
        vhm_CleanEngine();
        vhm_Clean();
        return 0;
    }
    return 1;
}
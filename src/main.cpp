#include "vhm_engine.h"
// #include "vhm_types.h"
#include "vhm_gl.h"

u32 vao;
u32 vbo;

void vhm_Init()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    f32 data[] = { 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1 }; 

    glBindVertexArray(vbo);
    vhm_SetBufferVAO(vao, vbo, 0, 2, data, GL_FLOAT, sizeof(data)/sizeof(f32));
}

void vhm_Clean()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void vhm_Update(f64 t, f64 dt)
{

}

void vhm_Draw()
{
    glBindVertexArray(vbo);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
}

int main()
{
    if(vhm_InitEngine() == VHM_SUCCESS)
    {
        vhm_Init();
        vhm_StartEngine(vhm_Update, vhm_Draw);
        vhm_CleanEngine();
        return 0;
    }
    return 1;
}
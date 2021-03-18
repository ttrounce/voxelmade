#ifndef VHM_GL_H
#define VHM_GL_H

#include "vhm_log.h"
#include "vhm_types.h"

/**
 * Fills specific VBO with data
 * @param handle VAO handle
 * @param vbo VBO handle
 * @param index vertex attrib array index
 * @param size the size of each "vertex", e.g.: 2 for 2D, 3 for 3D
 * @param data pointer to the data
 * @param length the amount of data
 */
void vhm_SetBufferVAO(u32 handle, u32 vbo, u32 index, u32 size, void* data, u32 type, u32 length);


// void blah()
// {
//     u32 handle = glCreateProgram();    
// }

void vhm_CreateVertexShader(u32 program, const char* source, i32 sourceLength);
void vhm_CreateFragmentShader(u32 program, const char* source, i32 sourceLength);
void vhm_LinkProgram(u32 program, u32* shaders, u32 shaderCount);

#endif // VHM_GL_H
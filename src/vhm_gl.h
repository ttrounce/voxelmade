#ifndef VHM_GL_H
#define VHM_GL_H

#include <glad/glad.h>
#include "vhm_types.h"

void vhm_SetBufferVAO(u32 handle, u32 vbo, u32 index, u32 size, void* data, u32 type, u32 length);

#endif // VHM_GL_H
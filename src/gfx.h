#ifndef VHM_GFX_H
#define VHM_GFX_H

#include "types.h"
#include <vector>

namespace vhm
{

// VAO/VBO

struct VAO
{
    u32 handle;
    u32 size;
    std::vector<u32> vbos;
};

void SetBufferVAO(VAO& vao, u32 index, u32 size, void* data, u32 type, u32 length);

// Shader Program

void CreateVertexShader(u32 program, const char* source, i32 sourceLength);
void CreateFragmentShader(u32 program, const char* source, i32 sourceLength);
void LinkProgram(u32 program);

void UniformVec2(u32 program, const char* name, vec2 vec);
void UniformVec3(u32 program, const char* name, vec3 vec);
void UniformVec4(u32 program, const char* name, vec4 vec);
void UniformMat2(u32 program, const char* name, mat2 value);
void UniformMat3(u32 program, const char* name, mat3 value);
void UniformMat4(u32 program, const char* name, mat4 value);

// Texture

void SetMinMag(u32 target, u32 min, u32 mag);
void LoadTexture(u32 handle, u8* data, u32 width, u32 height, u32 internalFormat, u32 format);
void LoadTextureArray(u32 handle, u8* data, u32 width, u32 height, u32 depth, u32 internalFormat, u32 format);
void SetTextureArrayStorage(u32 handle, u32 internalFormat, u32 width, u32 height, u32 depth);
void LoadTextureArrayLayer(u32 handle, u8* data, u32 x, u32 y, u32 width, u32 height, u32 layer, u32 format);

}

#endif // VHM_GFX_H
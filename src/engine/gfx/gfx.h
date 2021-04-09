#ifndef VHM_GFX_H
#define VHM_GFX_H

#include "../utility/types.h"
#include <vector>
#include <glad/glad.h>

namespace vhm
{

constexpr const f32 GFX_LAYER_TEXT = -0.0;
constexpr const f32 GFX_LAYER_GUI = -1.0;

// VAO/VBO

struct VAO
{
    u32 handle;
    u32 size;
    std::vector<u32> vbos;
};

void SetBufferVAO(vhm::VAO& vao, GLuint index, GLint size, void* data, GLenum type, GLint length);

// Shader Program

void CreateVertexShader(GLuint program, const GLchar* source, GLint sourceLength);
void CreateFragmentShader(GLuint program, const GLchar* source, GLint sourceLength);
void LinkProgram(GLuint program);

void UniformVec2(GLuint program, const GLchar* name, vec2 vec);
void UniformVec3(GLuint program, const GLchar* name, vec3 vec);
void UniformVec4(GLuint program, const GLchar* name, vec4 vec);
void UniformMat2(GLuint program, const GLchar* name, mat2 value);
void UniformMat3(GLuint program, const GLchar* name, mat3 value);
void UniformMat4(GLuint program, const GLchar* name, mat4 value);

// Texture

void SetMinMag(GLenum target, GLint min, GLint mag);
void LoadTexture(GLuint handle, void* data, GLsizei width, GLsizei height, GLint internalFormat, GLenum format);
void LoadTextureArray(GLuint handle, void* data, GLsizei width, GLsizei height, GLsizei depth, GLint internalFormat, GLenum format);
void SetTextureArrayStorage(GLuint handle, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
void LoadTextureArrayLayer(GLuint handle, void* data, GLint x, GLint y, GLsizei width, GLsizei height, GLint layer, GLenum format);
u32 CreateQuickTexture(GLuint target, GLuint* textureHandle, const char* path, GLint min, GLint mag);

}

#endif // VHM_GFX_H
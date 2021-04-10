#ifndef VHM_GFX_H
#define VHM_GFX_H

#include <glm/ext/vector_common.hpp>
#include <glm/ext/matrix_common.hpp>
#include "../utility/types.h"
#include <vector>
#include <string>
#include <glad/glad.h>

namespace vhm
{

constexpr const float GFX_LAYER_TEXT = -0.0;
constexpr const float GFX_LAYER_GUI = -1.0;

// VAO/VBO

struct VAO
{
    uint handle;
    uint size;
    std::vector<uint> vbos;
};

void SetBufferVAO(vhm::VAO& vao, GLuint index, GLint size, void* data, GLenum type, GLint length);

// Shader Program

void CreateVertexShader(GLuint program, std::string source, GLint sourceLength);
void CreateFragmentShader(GLuint program, std::string source, GLint sourceLength);
void LinkProgram(GLuint program);

void UniformVec2(GLuint program, std::string name, glm::vec2 vec);
void UniformVec3(GLuint program, std::string name, glm::vec3 vec);
void UniformVec4(GLuint program, std::string name, glm::vec4 vec);
void UniformMat2(GLuint program, std::string name, glm::mat2 value);
void UniformMat3(GLuint program, std::string name, glm::mat3 value);
void UniformMat4(GLuint program, std::string name, glm::mat4 value);

// Texture

void SetMinMag(GLenum target, GLint min, GLint mag);
void LoadTexture(GLuint handle, void* data, GLsizei width, GLsizei height, GLint internalFormat, GLenum format);
void LoadTextureArray(GLuint handle, void* data, GLsizei width, GLsizei height, GLsizei depth, GLint internalFormat, GLenum format);
void SetTextureArrayStorage(GLuint handle, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
void LoadTextureArrayLayer(GLuint handle, void* data, GLint x, GLint y, GLsizei width, GLsizei height, GLint layer, GLenum format);
uint CreateQuickTexture(GLuint target, GLuint* textureHandle, std::string path, GLint min, GLint mag);

}

#endif // VHM_GFX_H
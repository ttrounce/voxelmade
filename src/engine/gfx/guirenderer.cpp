#include "../utility/basicio.h"
#include "../utility/glm.h"
#include "../utility/color.h"
#include "../engine.h"
#include "guirenderer.h"

#include <string.h>
#include <stdlib.h>
#include <glad/glad.h>

using namespace vhm;

GUI_RENDERER::GUI_RENDERER()
{
    f32 vertices[12] = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    f32 texels[12] = {0.0, 0.0, 0.99, 0.99, 0.99, 0.0, 0.0, 0.0, 0.0, 0.99, 0.99, 0.99};

    glGenVertexArrays(1, &vao.handle);
    glBindVertexArray(vao.handle);
    SetBufferVAO(vao, 0, 2, vertices, GL_FLOAT, 12);
    SetBufferVAO(vao, 1, 2, texels, GL_FLOAT, 12);
    glBindVertexArray(0);

    char* vert = ReadString("shaders/gui_vert.glsl");
    char* frag = ReadString("shaders/gui_frag.glsl");

    program = glCreateProgram();
    glUseProgram(program);
    CreateVertexShader(program, vert, strlen(vert));
    CreateFragmentShader(program, frag, strlen(frag));
    LinkProgram(program);
    glUseProgram(0);

    free(vert);
    free(frag);
}

GUI_RENDERER::~GUI_RENDERER()
{
    glDeleteProgram(program);
    glDeleteBuffers(vao.vbos.size(), &vao.vbos[0]);
    glDeleteVertexArrays(1, &vao.handle);
}

void GUI_RENDERER::FillRect(i32 sx, i32 sy, i32 width, i32 height, COLOR color)
{
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    
    glUniform1i(glGetUniformLocation(program, "hasTexture"), false);

    glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI);

    // projection
    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0.0, (f32) GetWindowWidth(), (f32) GetWindowHeight(), 0.0, 0.0001, 10.0, projection);
    UniformMat4(program, "projection", projection);

    mat4 transformation;
    vec3 translation = {(f32) sx, (f32) sy, 0.0};
    vec3 scale = {(f32) width, (f32) height, 1.0};
    glm_mat4_identity(transformation);
    glm_translate(transformation, translation);
    glm_scale(transformation, scale);
    UniformMat4(program, "transformation", transformation);

    UniformVec3(program, "color", color.vec);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(0);
    glUseProgram(0);
}

void GUI_RENDERER::TextureRect(i32 sx, i32 sy, i32 width, i32 height, u32 textureHandleGL)
{
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    glBindTexture(GL_TEXTURE_2D, textureHandleGL);

    glUniform1i(glGetUniformLocation(program, "hasTexture"), true);

    glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI);

    // projection
    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0.0, (f32) GetWindowWidth(), (f32) GetWindowHeight(), 0.0, 0.0001, 10.0, projection);
    UniformMat4(program, "projection", projection);

    mat4 transformation;
    vec3 translation = {(f32) sx, (f32) sy, 0.0};
    vec3 scale = {(f32) width, (f32) height, 1.0};
    glm_mat4_identity(transformation);
    glm_translate(transformation, translation);
    glm_scale(transformation, scale);
    UniformMat4(program, "transformation", transformation);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
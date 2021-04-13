#include "../utility/glm.h"
#include "../utility/color.h"
#include "../utility/io.h"
#include "../engine.h"
#include "guirenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <string.h>
#include <stdlib.h>
#include <glad/glad.h>

using namespace vhm;

std::unique_ptr<GUI_RENDERER> vhm::guiRenderer = std::make_unique<GUI_RENDERER>();

GUI_RENDERER::GUI_RENDERER()
{
    float vertices[12] = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    float texels[12] = {0.0, 0.0, 0.99, 0.99, 0.99, 0.0, 0.0, 0.0, 0.0, 0.99, 0.99, 0.99};

    glGenVertexArrays(1, &vao.handle);
    glBindVertexArray(vao.handle);
    SetBufferVAO(vao, 0, 2, vertices, GL_FLOAT, 12);
    SetBufferVAO(vao, 1, 2, texels, GL_FLOAT, 12);
    glBindVertexArray(0);

    std::string vert = ReadString("shaders/gui_vert.glsl");
    std::string frag = ReadString("shaders/gui_frag.glsl");

    program = glCreateProgram();
    glUseProgram(program);
    CreateVertexShader(program, vert, vert.length());
    CreateFragmentShader(program, frag, frag.length());
    LinkProgram(program);
    glUseProgram(0);
}

GUI_RENDERER::~GUI_RENDERER()
{
    glDeleteProgram(program);
    glDeleteBuffers(vao.vbos.size(), &vao.vbos[0]);
    glDeleteVertexArrays(1, &vao.handle);
}

void GUI_RENDERER::FillRect(int sx, int sy, int width, int height, COLOR color)
{
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    
    glUniform1i(glGetUniformLocation(program, "hasTexture"), false);

    glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI);

    // projection
    glm::mat4 projection = glm::ortho<float>(0.0, (float) GetWindowWidth(), (float) GetWindowHeight(), 0.0, 0.0001, 10.0);
    UniformMat4(program, "projection", projection);

    glm::mat4 transformation(1.0f);
    transformation = glm::translate(transformation, glm::vec3(sx, sy, 0.0));
    transformation = glm::scale(transformation, glm::vec3(width, height, 1.0));
    UniformMat4(program, "transformation", transformation);

    UniformVec3(program, "color", color.vec);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(0);
    glUseProgram(0);
}

void GUI_RENDERER::TextureRect(int sx, int sy, int width, int height, uint textureHandleGL)
{
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    glBindTexture(GL_TEXTURE_2D, textureHandleGL);

    glUniform1i(glGetUniformLocation(program, "hasTexture"), true);

    glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI);

    // projection
    glm::mat4 projection = glm::ortho<float>(0.0, (float) GetWindowWidth(), (float) GetWindowHeight(), 0.0, 0.0001, 10.0);
    UniformMat4(program, "projection", projection);

    glm::mat4 transformation(1.0f);
    transformation = glm::translate(transformation, glm::vec3(sx, sy, 0.0));
    transformation = glm::scale(transformation, glm::vec3(width, height, 1.0));
    UniformMat4(program, "transformation", transformation);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
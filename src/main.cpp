#include "engine.h"
#include "font.h"
#include "camera.h"
#include "engine_io.h"
#include "gfx.h"

#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace vhm;

vec3 COL_BLACK = {0.0, 0.0, 0.0};

u32 terrainProgram;

PERSPECTIVE_CAMERA* camera;

#include <chrono>

void Init()
{
    InitFreeType();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    char* vert = ReadString("terrain.vert");
    char* frag = ReadString("terrain.frag");
    terrainProgram = glCreateProgram();
    CreateVertexShader(terrainProgram, vert, strlen(vert));
    CreateFragmentShader(terrainProgram, frag, strlen(frag));
    LinkProgram(terrainProgram);
    free(vert);
    free(frag);

    camera = new PERSPECTIVE_CAMERA(GetAspectRatio(), 70.0, 0.01, 1000.0);

    FONT_RENDERER::GetInstance()->DeriveFace(24);
}

void Clean()
{
    free((void*) camera);
    FreeFreeType();
}

void Update(f64 time, f64 dt)
{
    PollCameraInputs(*camera);
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(col(110.0), col(207.0), col(225.0), 1.0);

    // glUseProgram(terrainProgram);

    // UpdatePerspectiveCamera(*camera);
    // UniformPerspectiveCamera(terrainProgram, "view", "projection", *camera);

    // glUseProgram(0);

    char* text = (char*) malloc(snprintf(NULL, 0, "%d", window->fps) + 1);
    sprintf(text, "%d", window->fps);
    free(text);

    FONT_RENDERER* fonter = FONT_RENDERER::GetInstance();

    fonter->RenderText(text, GetWindowWidth() - fonter->TextLength(text) - 5, 5, COL_BLACK);
}

int main()
{
    if(InitEngine() == VHM_SUCCESS)
    {
        Init();
        StartEngine(Update, Draw);
        CleanEngine();
        Clean();
        return 0;
    }
    return 1;
}
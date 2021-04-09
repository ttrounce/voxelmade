#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/gfx/font.h"
#include "engine/gfx/gfx.h"
#include "engine/gfx/guirenderer.h"

#include "engine/utility/color.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace vhm;

constexpr char* FONT_HACK = (char*) "hack";

PERSPECTIVE_CAMERA* camera;

void Init()
{
    InitFreeType();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    camera = new PERSPECTIVE_CAMERA(GetAspectRatio(), 70.0, 0.01, 1000.0);

    FONT_RENDERER::GetInstance()->LoadNewFace(FONT_HACK, "fonts/hack.ttf", 24);
}

void Clean()
{
    delete camera;
    FreeFreeType();
}


void Update(f64 time, f64 dt)
{
    PollCameraInputs(*camera);
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(110.0/255.0, 207.0/255.0, 225.0/255.0, 1.0);

    // glUseProgram(terrainProgram);

    // UpdatePerspectiveCamera(*camera);
    // UniformPerspectiveCamera(terrainProgram, "view", "projection", *camera);

    // glUseProgram(0);

    char* text = (char*) malloc(snprintf(NULL, 0, "%d", window->fps) + 1);
    sprintf(text, "%d", window->fps);

    FONT_RENDERER* fonter = FONT_RENDERER::GetInstance();
    fonter->RenderText(FONT_HACK, text, 5, 5, COLOR(0x000000));
    fonter->RenderText(FONT_HACK, "VOXELMADE", (GetWindowWidth() - fonter->TextWidth(FONT_HACK, "VOXELMADE")) / 2.0, 5, COLOR(0x000000));
    free(text);
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
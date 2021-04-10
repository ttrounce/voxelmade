#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/audio/audio.h"
#include "engine/gamestate.h"
#include "engine/gfx/font.h"
#include "engine/gfx/gfx.h"
#include "engine/gfx/guirenderer.h"
#include "engine/utility/color.h"

#include "game/gamestates/playstate.h"
#include "game/gamestates/menustate.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <memory>

using namespace vhm;

std::unique_ptr<GAME_STATE_MANAGER> gameStateManager;

void Init()
{
    InitFreeType();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    gameStateManager = std::make_unique<GAME_STATE_MANAGER>();
    gameStateManager->RegisterState("menu", std::make_unique<GAME_STATE_MENU>());
    gameStateManager->RegisterState("play", std::make_unique<GAME_STATE_PLAY>());
    gameStateManager->SetState("menu");
}

void Clean()
{
    // Force managers to destruct.
    fontRenderer.reset();
    audioManager.reset();
    gameStateManager.reset();
    FreeFreeType();
}


void Update(double time, double dt)
{
    gameStateManager->UpdateState(time, dt);
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gameStateManager->DrawState();
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
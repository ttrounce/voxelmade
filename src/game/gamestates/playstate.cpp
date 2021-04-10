#include "../../engine/utility/color.h"
#include "../../engine/gfx/font.h"
#include "../../engine/engine.h"
#include "playstate.h"
#include <iostream>

using namespace vhm;

void GAME_STATE_PLAY::OnStart(GAME_STATE_MANAGER& manager)
{

}

void GAME_STATE_PLAY::OnStop(GAME_STATE_MANAGER& manager)
{

}

void GAME_STATE_PLAY::OnUpdate(GAME_STATE_MANAGER& manager, double t, double dt)
{

}

void GAME_STATE_PLAY::OnDraw(GAME_STATE_MANAGER& manager)
{
    glClearColor(110.0/255.0, 207.0/255.0, 225.0/255.0, 1.0);
    fontRenderer->RenderText("hack", "oh shit", 5, 5, COLOR(0x000000));
}

GAME_STATE_PLAY::GAME_STATE_PLAY()
{

}

GAME_STATE_PLAY::~GAME_STATE_PLAY()
{

}
#include "../../engine/utility/color.h"
#include "../../engine/audio/audio.h"
#include "../../engine/gfx/font.h"
#include "../../engine/engine.h"
#include "menustate.h"

using namespace vhm;

void GAME_STATE_MENU::OnStart(GAME_STATE_MANAGER& manager)
{
    manager.SetState("play");
}

void GAME_STATE_MENU::OnStop(GAME_STATE_MANAGER& manager)
{

}

void GAME_STATE_MENU::OnUpdate(GAME_STATE_MANAGER& manager, double t, double dt)
{

}

void GAME_STATE_MENU::OnDraw(GAME_STATE_MANAGER& manager)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    fontRenderer->RenderText("hack-48", "V O X E L M A D E", (GetWindowWidth() - fontRenderer->TextWidth("hack-48", "V O X E L M A D E")) / 2.0, GetWindowHeight() / 4.0, COLOR(0xFFFFFF));
}

GAME_STATE_MENU::GAME_STATE_MENU()
{
    fontRenderer->LoadFace("hack", "fonts/hack.ttf", 24);
}

GAME_STATE_MENU::~GAME_STATE_MENU()
{

}
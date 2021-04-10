#include "../../engine/gamestate.h"

namespace vhm
{

class GAME_STATE_PLAY : public GAME_STATE
{
    public:
        void OnStart(GAME_STATE_MANAGER& manager);
        void OnStop(GAME_STATE_MANAGER& manager);
        void OnUpdate(GAME_STATE_MANAGER& manager, double t, double dt);
        void OnDraw(GAME_STATE_MANAGER& manager);

        GAME_STATE_PLAY();
        ~GAME_STATE_PLAY();
};

}
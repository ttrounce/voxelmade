#ifndef VHM_GAMESTATE_H
#define VHM_GAMESTATE_H

#include <map>
#include "utility/types.h"

namespace vhm
{

class GAME_STATE_MANAGER;

class GAME_STATE
{
    public:
        void virtual OnStart(GAME_STATE_MANAGER& manager) = 0;
        void virtual OnUpdate(GAME_STATE_MANAGER& manager, double t, double dt) = 0;
        void virtual OnDraw(GAME_STATE_MANAGER& manager) = 0;
        void virtual OnStop(GAME_STATE_MANAGER& manager) = 0;
        GAME_STATE() {}
        virtual ~GAME_STATE() {}
};

class GAME_STATE_MANAGER
{
    private:
        std::map<std::string, std::unique_ptr<GAME_STATE>> states;
        std::string currentStateKey;
    public:
        void RegisterState(std::string key, std::unique_ptr<GAME_STATE>&& state)
        {
            states.insert(std::make_pair(key, std::move(state)));
        }
        void SetState(std::string key)
        {
            if(states.count(currentStateKey) != 0)
            {
                states.at(currentStateKey)->OnStop(*this);
            }
            currentStateKey = key;
            states.at(currentStateKey)->OnStart(*this);
        }
        void UpdateState(double t, double dt)
        {
            if(states.count(currentStateKey) != 0)
            {
                states.at(currentStateKey)->OnUpdate(*this, t, dt);
            }
        }
        void DrawState()
        {
            if(states.count(currentStateKey) != 0)
            {
                states.at(currentStateKey)->OnDraw(*this);
            }
        }
        GAME_STATE_MANAGER() {}
        ~GAME_STATE_MANAGER() {}
};
    
}

#endif // VHM_GAMESTATE_H
#ifndef DANSQD_GAMESTATE_H
#define DANSQD_GAMESTATE_H

#include "config.h"
#include "utils/localUtil.h"

struct GSRet {
    VM::FMinimalViewInfo vm{};
    std::vector<PlayerEnt> ents{};
    std::vector<VehicalEnt> vents{};
    int LPteam{};
};

class GameState {
private:
    ptr gameBase{};

public:
    GameState(ptr gameBase = 0x140000000);
    GSRet tick();

private:
    ptr getUworld();

    std::vector<PlayerEnt> getEnts(ptr uworld);

    std::vector<VehicalEnt> getVehInfo(ptr uworld);

    struct LPRet {
        VM::FMinimalViewInfo vm{};
        int teamID{};
    };
    LPRet getLPInfo(ptr uworld);

};



#endif //DANSQD_GAMESTATE_H
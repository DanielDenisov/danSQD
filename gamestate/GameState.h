#ifndef DANSQD_GAMESTATE_H
#define DANSQD_GAMESTATE_H

#include "config.h"
#include "utils/localUtil.h"

struct GSRet {

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


};



#endif //DANSQD_GAMESTATE_H
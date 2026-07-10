#pragma once

#ifndef DANSQD_LOCALUTIL_H
#define DANSQD_LOCALUTIL_H

#include "config.h"
#include "gameUtil.h"

struct PlayerEnt {
    ptr memoryID{};
    int32_t teamID{};
    FVector pos{};
    int health{100};

};

#endif //DANSQD_LOCALUTIL_H
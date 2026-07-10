#pragma once

#ifndef DANSQD_LOCALUTIL_H
#define DANSQD_LOCALUTIL_H

#include "config.h"
#include "gameUtil.h"

struct PlayerEnt {
    ptr memoryID{};
    int32_t teamID{};
    FVector pos{};
    float health{100.f};
    std::string username{};
    PlayerStateFlags plFlags{};
    bool isCrouched{};
    bool isProne{};
};

struct Vector2 {
    double x{}, y{};

    void Print() {
        std::cout << "V2: " << x << " " << y << std::endl;
    }
};

#endif //DANSQD_LOCALUTIL_H
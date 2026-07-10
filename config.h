#pragma once
#ifndef DANSQD_CONFIG_H
#define DANSQD_CONFIG_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>
//do not include local files here

namespace config {
    const inline int SCREEN_H = 1405;
    const inline int SCREEN_W = 2560;

    const inline int FarthestPlayerDist = 400;

    inline const auto OVERLAY_NAME = "Better Discord Overlay";
}

#define ptr uint64_t
#define pr inline ptr

inline constexpr bool isDebugMode = true;

#define DBG if(!isDebugMode) {} else
struct asHex {
    const int& value;
    friend std::ostream& operator<<(std::ostream& os, const asHex& ah) {
        return os << std::hex << ah.value << std::dec;
    }
};

namespace off {
    pr UWORLD = 0xd1c9eb8;

    //Get Player Path
    //UWorld -> GameState
    pr GAME_STATE = 0x1b0;
    //AGameStateBase->PlayerArray
    pr PLAYER_ARRAY = 0x2d0; //Of APlayerState, use PAWN to get pawn



    //for getting info like is about if is a bot or spectator and stuff
    //APlayerState->bShouldUpdateReplicatedPing
    pr PL_bShouldUpdateReplicatedPing = 0x2C2; //read as type PlayerStateFlags
    //APlayerState->PlayerNamePrivate
    pr PL_PLAYER_NAME_PRIVATE = 0x358;
    //APlayerState->PrivatePawn
    pr PL_PAWN = 0x338;
    //ASQPlayerState->TeamId
    pr PL_TEAM_ID = 0x508; //read from playerstate
    //TO ADD: Current weapon would be somewhere here prob

    //Things Gotten from reading pawn
    //AActor->0x1c0
    pr PW_ROOT_COMP = 0x1C0;
    //USceneComponent->RelativeLocation
    pr PW_POS = 0x148;
    //TO ADD: If doing barrel ESP, would read rot, scale, pitch from here
    //ASQSoldier->Health
    pr PW_HEALTH = 0x2740; //float

    //For reading local player info
    //UWorld->OwningGameInstance
    pr OWNING_GAME_INST = 0x230;
    //UGameInstance->LocalPlayers
    pr LOCAL_PLAYERS = 0x38; //list, read list, then read result (reading index 0 of list)
    //UPlayer->PlayerController
    pr PLAYER_CONTROLLER = 0x30;
    //For Reading TeadId of the
    //AController->PlayerState
    pr CT_PlayerState = 0x2C0;
    //can now read localplayer team ID

    //APlayerController->PlayerCameraManager
    pr PL_PLAYER_CAM_MANAGER = 0x388;
    //APlayerCameraManager->CameraCachePrivate;
    pr CAM_CACHE_PRIVATE = 0x15b0; //type FCameraCacheEntry

    //Fun info from pawn in ASQSoldier


}

namespace vtable {

}

#endif //DANSQD_CONFIG_H
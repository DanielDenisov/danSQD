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

inline constexpr bool isDebugMode = false;

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
    //ACharacter->bIsCrouched
    pr PW_IS_CROUCHED = 0x460;
    //ASQSoldier->bIsProne
    pr PW_IS_PRONE = 0x1F4C;

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
    //APlayerController->AcknowledgedPawn
    pr CT_ACKPAWN = 0x378;
    //AAcatar_C->Aim_Weaon
    pr AIM_WEAPON = 0x8C8; //bool

    //APlayerController->PlayerCameraManager
    pr PL_PLAYER_CAM_MANAGER = 0x388;
    //APlayerCameraManager->CameraCachePrivate;
    pr CAM_CACHE_PRIVATE = 0x15b0; //type FCameraCacheEntry

    //Uworld->PersistentLevel
    pr PRESISTENT_LVL = 0x30;
    //Either 0x98 to 0xA0
    pr PL_ACTOR_ARRAY = 0x98;
    //only for vehicals
    //ASQVehicle->ClaimedBySquad
    pr CLAIMED_BY_SQUAD = 0x670; //type ASQSquadState
    //ASQSquadState->TeamId
    pr SQ_TEAM_ID = 0x34C;

    //ASQVehicle->VehicleType
    pr VehicalType = 0x7e0;


}


// Unique scope magnification levels in Squad (Sorted smallest to largest)
inline std::vector<double> squadMagnifications = {
    1.0,   // Variable default (SpecterDR, 1P87, A940 base zoom)
    2.8,   // 1P78 Kashtan (RGF/VDV)
    3.0,   // QMK-152 (PLA Chinese Infantry)
    3.4,   // Elcan C79 (CAF Canadian Infantry)
    3.5,   // PU Vintage Scope (Mosin Nagant)
    4.0,   // Standard ACOG / Max zoom on variables / PSO-1 / 1P29 / G3 ZF
    6.0,   // Universal Marksman / DMR zoom standard
    8.0,   // USMC M27 SDO / C14 Timberwolf low-zoom
    10.0,  // Standard Heavy Sniper zoom (M40A6, L115A3, CS/LR4, Hyperion)
    16.0   // C14 Timberwolf max zoom (CAF Heavy Sniper)
};


namespace vtable {

}

#endif //DANSQD_CONFIG_H
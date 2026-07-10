#include "GameState.h"

#include <thread>

#include "memory/memory.h"

GameState::GameState(uint64_t gameBase) {
    this->gameBase = gameBase;
}

GSRet GameState::tick() {
    ptr uworld = getUworld();
    if (!uworld) {
        std::cout << "[-] Uworld not found"  << std::endl;
        return {};
    }
    DBG{std::cout << "[+] Found uworld at " << std::hex << uworld << std::dec << std::endl;}

    std::vector<PlayerEnt> ents = getEnts(uworld);
    if (ents.empty()) {
        std::cout << "[-] 0 Entities found"  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return {};
    }
    DBG{std::cout << "[+] Found " << ents.size() << " entities" << std::endl;}
    ents.push_back(PlayerEnt{0, 10, {-27520, 8220, 1400}, 80, "guy", {}});

    LPRet lpret = getLPInfo(uworld);
    if (lpret.vm.FOV == 0) {
        std::cout << "[-] Failed to find view matrix" << std::endl;
        return {};
    }
    DBG{lpret.vm.Print();}

    return {lpret.vm, ents, lpret.teamID};
}

uint64_t GameState::getUworld() {
    ptr uworld = ReadMemory<ptr>(this->gameBase + off::UWORLD);

    return uworld;
}

std::vector<PlayerEnt> GameState::getEnts(ptr uworld) {
    std::vector<PlayerEnt> eret{};

    ptr gameState = ReadMemory<ptr>(uworld + off::GAME_STATE);
    if (!gameState) {
        std::cout << "[-] Failed to find gamestate" << std::endl;
        return{};
    }
    DBG{std::cout << "[+] Found gamestate at 0x" << std::hex << gameState << std::dec << std::endl;}

    ptr playerArray = ReadMemory<ptr>(gameState + off::PLAYER_ARRAY);
    int PACount = ReadMemory<int>(gameState + off::PLAYER_ARRAY + sizeof(ptr));
    if (!playerArray || PACount == 0) {
        std::cout << "[-] Failed to find playerArray with count " << PACount << std::endl;
        return {};
    }
    DBG{std::cout << "[+] Found playerArray at 0x" << std::hex << playerArray << std::dec << " with count " << PACount << std::endl;}

    for (int a{}; a < PACount; a++) {
        ptr plState = ReadMemory<ptr>(playerArray + a * sizeof(ptr));
        if (!plState) continue;

        PlayerEnt ent{};
        ent.memoryID = plState;

        //Get Team Info
        ent.teamID = ReadMemory<int32_t>(plState + off::PL_TEAM_ID);

        //Get Username
        ent.username = ReadFString(plState + off::PL_PLAYER_NAME_PRIVATE);

        //Get Flags
        ent.plFlags = ReadMemory<PlayerStateFlags>(plState + off::PL_bShouldUpdateReplicatedPing);


        ptr pawn = ReadMemory<ptr>(plState + off::PL_PAWN);
        if (!pawn) continue;

        //Get Health
        ent.health = ReadMemory<float>(pawn + off::PW_HEALTH);

        ptr rootComp = ReadMemory<ptr>(pawn + off::PW_ROOT_COMP);

        //Get Position info
        ent.pos = ReadMemory<FVector>(rootComp + off::PW_POS);
        if (ent.pos.Dist(FVector{}) < 10) continue;


        eret.push_back(ent);
    }

    return eret;
}

GameState::LPRet GameState::getLPInfo(uint64_t uworld) {
    LPRet lpret;
    ptr owngame = ReadMemory<ptr>(uworld + off::OWNING_GAME_INST);
    if (!owngame) {
        std::cout << "[-] Failed to find owning game inst" << std::endl;
        return{};
    }
    DBG{std::cout << "[+] Found owning game inst at 0x" << std::hex << owngame << std::dec << std::endl;}

    ptr localPlayers = ReadMemory<ptr>(owngame + off::LOCAL_PLAYERS);
    ptr localPlayer = ReadMemory<ptr>(localPlayers); //because only 1
    if (!localPlayer) {
        std::cout << "[-] Failed to find localPlayer" << std::endl;
        return{};
    }
    DBG{std::cout << "[+] Found localPlayer at 0x" << std::hex << localPlayer << std::dec << std::endl;}

    ptr playerController = ReadMemory<ptr>(localPlayer + off::PLAYER_CONTROLLER);
    if (!playerController) {
        std::cout << "[-] Failed to find playerController" << std::endl;
        return{};
    }
    DBG{std::cout << "[+] Found playerController at 0x" << std::hex << playerController << std::dec << std::endl;}

    ptr playerState = ReadMemory<ptr>(playerController + off::CT_PlayerState);
    DBG{std::cout << "[+] Found playerState at 0x" << std::hex << playerState << std::dec << std::endl;}
    if (playerState) {
        lpret.teamID = ReadMemory<int32_t>(playerState + off::PL_TEAM_ID);
        DBG{std::cout << "[+] LP TeamID: " << lpret.teamID << std::endl;}
    } else { DBG{std::cout << "[-] No Local Player State found" << std::endl;} }


    ptr camManager = ReadMemory<ptr>(playerController + off::PL_PLAYER_CAM_MANAGER);
    if (!camManager) {
        std::cout << "[-] Failed to find camManager" << std::endl;
        return{};
    }
    DBG{std::cout << "[+] Found camManager at 0x" << std::hex << camManager << std::dec << std::endl;}

    VM::FCameraCacheEntry vmtemp = ReadMemory<VM::FCameraCacheEntry>(camManager + off::CAM_CACHE_PRIVATE);
    lpret.vm = vmtemp.viewInfo;

    return lpret;
}

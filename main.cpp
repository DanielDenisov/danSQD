#include <iostream>
#include "memory/memory.h"

int main() {
    ProcessId = FindGamePID();
    if (!ProcessId) {
        std::cerr << "[-] Could not find game PID, is the game running?" << std::endl;
        return 0;
    }
}

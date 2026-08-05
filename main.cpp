#include <atomic>
#include <csignal>

#include "draw/ESP.h"
#include "draw/Overlay.h"
#include "gamestate/GameState.h"
#include "memory/memory.h"
#include "utils/InputReader.h"

inline std::atomic<bool> g_Running(true);
void SignalHandler(int signum) {g_Running = false;}

int main() {

    InputReader inputReader;
    if (!inputReader.Init()) {
        std::cerr << "[-] Failed to init global input reader" << std::endl;
        return 1;
    }

    std::signal(SIGINT, SignalHandler);
    if (!InitOverlay()) {
        std::cerr << "[-] Failed to init Overlay (GLFW/Wayland)" << std::endl;
        return 1;
    }

    ProcessId = FindGamePID();
    if (!ProcessId) {
        std::cerr << "[-] Could not find game PID, is the game running?" << std::endl;
        return 0;
    }

    // Tracked in tenths (int) rather than as a float incremented by 0.1 directly,
    // so repeated +/- presses can't drift off the 0.1 grid via float rounding error.
    int scopeTenths = 10; // 1.0x

    GameState gs = GameState();
    while (g_Running && !glfwWindowShouldClose(window)) {
        GSRet gsr = gs.tick();

        RenderBegin();

        char buf[64];
        sprintf(buf, "Entities: %lu", gsr.ents.size());
        DrawTextImGui(10, 10, IM_COL32(255, 0, 0, 255), buf);
        inputReader.Poll();

        double scopeMagnification = scopeTenths / 10.0;
        if (!inputReader.IsKeyDown(KEY_8)) {
            ESP(gsr.vm, gsr.ents, gsr.vents, gsr.LPteam, scopeMagnification, inputReader.IsRMBDown());
        }

        // WasKeyPressed() alone fires only on the press edge; the previous
        // `IsKeyDown() && !WasKeyPressed()` check did the opposite - it fired on
        // every held frame *except* the first, so holding the key spammed the
        // step every frame and could blow straight through to the min/max.
        if (inputReader.WasKeyPressed(KEY_LEFTBRACE)) {
            scopeTenths -= 1;
            if (scopeTenths < 10) scopeTenths = 10; // min 1.0x
        }

        if (inputReader.WasKeyPressed(KEY_RIGHTBRACE)) {
            scopeTenths += 1;
            if (scopeTenths > 160) scopeTenths = 160; // max 16.0x
        }

        char bufscope[64];
        sprintf(bufscope, "Scope: %.1fx", scopeMagnification);
        DrawTextImGui(10, 30, IM_COL32(255, 0, 0, 255), bufscope);

        RenderEnd();
    }
    std::cout << "[+] Destructing Window" << std::endl;

    if (ImGui::GetCurrentContext()) {
        if (ImGui::GetIO().BackendRendererUserData)
            ImGui_ImplOpenGL3_Shutdown();

        if (ImGui::GetIO().BackendPlatformUserData)
            ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();
    }

    if (window) {
        glfwHideWindow(window);
        glfwDestroyWindow(window);
    }
    return 0;
}

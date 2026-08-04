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

    int scopeIndex = 0;

    GameState gs = GameState();
    while (g_Running && !glfwWindowShouldClose(window)) {
        GSRet gsr = gs.tick();

        RenderBegin();

        char buf[64];
        sprintf(buf, "Entities: %lu", gsr.ents.size());
        DrawTextImGui(10, 10, IM_COL32(255, 0, 0, 255), buf);
        inputReader.Poll();
        if (!inputReader.IsKeyDown(KEY_8)) {
            ESP(gsr.vm, gsr.ents, gsr.vents, gsr.LPteam, squadMagnifications[scopeIndex], inputReader.IsRMBDown());
        }

        if (inputReader.IsKeyDown(KEY_LEFTBRACE) &&
            !inputReader.WasKeyPressed(KEY_LEFTBRACE)) {
            scopeIndex--;
            if (scopeIndex < 0) scopeIndex = 0;
        }

        if (inputReader.IsKeyDown(KEY_RIGHTBRACE) &&
            !inputReader.WasKeyPressed(KEY_RIGHTBRACE)) {
            scopeIndex++;
            if (scopeIndex > squadMagnifications.size()-1) scopeIndex = squadMagnifications.size()-1;
        }

        char bufscope[64];
        sprintf(bufscope, "Scope: %.1fx", squadMagnifications[scopeIndex]);
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

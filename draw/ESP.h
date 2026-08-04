#ifndef DANSQD_ESP_H
#define DANSQD_ESP_H

#include "config.h"
#include "Overlay.h"
#include "utils/localUtil.h"

Vector2 WorldToScreen(FVector TargetLocation, VM::FMinimalViewInfo CameraInfo, float ScreenWidth, float ScreenHeight);

inline double getProportionalGap(double dist, double maxDist) {
    return (1-(dist/maxDist)) * 6;
}

namespace COLOR {
#define IColor inline ImU32
    inline ImU32 withAlpha(ImU32 color, int alpha) {
        return (color & 0x00FFFFFF) | ((ImU32)(alpha & 0xFF) << 24);
    }
    inline int TRANS_VAL{150};

    IColor RED = IM_COL32(207, 56, 56, 255);
    IColor RED_TRANS = withAlpha(RED, TRANS_VAL);
    IColor GREEN = IM_COL32(34, 189, 39, 255);
    IColor GREEN_TRANS = withAlpha(GREEN, TRANS_VAL);
    IColor BLUE = IM_COL32(43, 145, 235, 255);
    IColor BLUE_TRANS = withAlpha(BLUE, TRANS_VAL);
    IColor WHITE = IM_COL32(255, 255, 255, 255);
    IColor WHITE_TRANS = withAlpha(WHITE, TRANS_VAL);

}

//retuens list of spectatorsbut basically a void
inline std::string DrawEnts(VM::FMinimalViewInfo vm, std::vector<PlayerEnt>& ents, std::vector<VehicalEnt>&, int LPteam, bool isScoped = false) {
    ImU32 healthColor = isScoped ? COLOR::BLUE_TRANS : COLOR::GREEN_TRANS;
    std::string spectators{};
    for (PlayerEnt ent : ents) {
        int dist = ent.pos.Dist(vm.Location) / 100; //cm -> m

        //check eligibility of player
        if (ent.health < 0.1f) continue;

        ImU32 color = COLOR::RED;
        if (ent.teamID == LPteam) color = COLOR::GREEN_TRANS;

        //Some weird inversion glitch here, but it prob does not matter too much
        // just makes the math look very wrong
        float edgeFromCenter = 90.0f;
        if (ent.isCrouched) edgeFromCenter *= 0.75; //reduce
        if (ent.isProne) edgeFromCenter *= 0.35; //reduce

        FVector head3d = ent.pos; head3d.z += edgeFromCenter;
        FVector feet3d = ent.pos; feet3d.z -= edgeFromCenter;
        Vector2 sfeet = WorldToScreen(feet3d, vm, config::SCREEN_W, config::SCREEN_H);
        Vector2 shead = WorldToScreen(head3d, vm, config::SCREEN_W, config::SCREEN_H);

        int height = sfeet.y - shead.y;
        int w = height/2;

        int gap = getProportionalGap(dist, config::FarthestPlayerDist);
        if (ent.teamID == LPteam) {
            // DrawCircleFilled(shead.x, shead.y - gap - 10, 5, color);
            continue;
        } else {
            // DrawBox(shead.x - w/2, shead.y, w, height, color);

            //health stuff
            int lineX = shead.x + w/2 + gap + 4;
            DrawLine(lineX, shead.y, lineX, sfeet.y, healthColor);
            if (ent.health < 100.f) {
                int YStop = shead.y + height-(height * (ent.health/100));
                DrawLine(lineX, shead.y, lineX, YStop, COLOR::RED_TRANS);
            }
        }


        char dBuf[64];
        sprintf(dBuf, "%.0im", dist);
        if (ent.isCrouched) sprintf(dBuf, "%.0im (C)", dist);
        if (ent.isProne) sprintf(dBuf, "%.0im (P)", dist);
        DrawTextCentered(shead.x, shead.y - gap - 4, COLOR::WHITE_TRANS, dBuf);

        // char pBuf[64];
        // sprintf(pBuf, "%s", ent.username.c_str());
        // DrawTextCentered(sfeet.x, sfeet.y + gap + 4, COLOR::WHITE_TRANS, pBuf);

        int txtStack = 0; //prevents text from stacking on itself
        int txtJump = 15;
        txtStack = txtJump + 4/*base*/;

        // if (ent.plFlags.bIsABot()) {
        //     DrawTextCentered(shead.x, shead.y - gap - txtStack, COLOR::WHITE, "(BOT)");
        //     txtStack += txtJump;
        // }
        //
        // if (ent.plFlags.bIsInactive()) {
        //     DrawTextCentered(shead.x, shead.y - gap - txtStack, COLOR::WHITE, "(INC)");
        //     txtStack += txtJump;
        // }

        if (ent.plFlags.bIsSpectator() || ent.plFlags.bOnlySpectator()) {
            int yLvl = shead.y - gap - txtStack;
            DrawLine(shead.x - w/2, yLvl, shead.x + w/2, yLvl, COLOR::WHITE, 10);
            DrawTextCentered(shead.x, yLvl, COLOR::RED, "(SPECTATOR)");
            txtStack += txtJump;

            spectators += ent.username + ", ";
        }
        // if (ent.plFlags.bIsABot()) {
        //     DrawTextCentered(shead.x, shead.y - gap - txtStack, COLOR::WHITE, "(BOT)");
        //     txtStack += txtJump;
        // }
        //
        // if (ent.plFlags.bIsInactive()) {
        //     DrawTextCentered(shead.x, shead.y - gap - txtStack, COLOR::WHITE, "(INC)");
        //     txtStack += txtJump;
        // }
    }
    return spectators;
}
inline void ESP(VM::FMinimalViewInfo vm, std::vector<PlayerEnt>& ents, std::vector<VehicalEnt>& vents, int LPteam, double scopeMagnification, bool isRMBDown) {
    // All of Overlay.h's Draw* helpers draw into the background draw list (a layer
    // rendered before/behind every ImGui window), not the current window's draw
    // list. ImGui::PushClipRect()/PopClipRect() only affect the *current window's*
    // draw list, so pushing a clip rect that way silently clips nothing here - we
    // have to push it directly onto the same ImDrawList the entities are drawn into.
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Define center anchors and scope glass geometry
    float centerX = config::SCREEN_W / 2.0f;
    float centerY = config::SCREEN_H / 2.0f;
    float scopeGlassRadius = static_cast<float>((int)centerY * 0.4);

    // Create a generated scope matrix using your dynamic magnification factor
    VM::FMinimalViewInfo scopeVM = vm;
    if (scopeMagnification > 1.01f) {
        scopeVM.FOV /= static_cast<float>(scopeMagnification);
    }

    // Allocate two separate entity vectors to isolate our visual environments
    std::vector<PlayerEnt> backgroundPlayers;
    std::vector<PlayerEnt> scopePlayers;

    // --- SORT ENEMIES BASED ON VISUAL SPACE ---
    for (const auto& ent : ents) {
        // Project everyone using the unmagnified baseline first to locate them in 2D space
        Vector2 sfeet = WorldToScreen(ent.pos, vm, config::SCREEN_W, config::SCREEN_H);

        // Calculate exact distance from the center of your monitor
        float distToCenter = std::sqrt(std::pow(sfeet.x - centerX, 2) + std::pow(sfeet.y - centerY, 2));

        if (isRMBDown && scopeMagnification > 1.01f && distToCenter < scopeGlassRadius) {
            // Target is physically inside the scope lens circle area -> Route to the scope list
            scopePlayers.push_back(ent);
        } else {
            // Target is in your peripheral vision around the lens -> Route to background list
            backgroundPlayers.push_back(ent);
        }
    }

    // --- STEP 1: DRAW THE UNMAGNIFIED BACKGROUND AREA ---
    // Only pass the players who are safely outside your lens perimeter bounds
    DrawEnts(vm, backgroundPlayers, vents, LPteam, false);

    // --- STEP 2: DRAW THE CLEAN MAGNIFIED SCOPE INTERIOR AREA ---
    if (isRMBDown && scopeMagnification > 1.01f) {

        // Magnification pushes scoped entities' projected positions outward from
        // center, so they can end up drawn well past the lens circle. Clip to the
        // square inscribed *inside* the circle (side = r*sqrt(2)) rather than the
        // square that circumscribes it - that guarantees nothing ever renders past
        // the visible lens boundary. Clipping to the circumscribed square instead
        // would let content bleed into the four corner slivers between the square
        // and the circle, which is exactly the leftover "erase outside" bug.
        float clipHalf = scopeGlassRadius / std::sqrt(2.0f);
        ImVec2 clipMin(centerX - clipHalf, centerY - clipHalf);
        ImVec2 clipMax(centerX + clipHalf, centerY + clipHalf);
        drawList->PushClipRect(clipMin, clipMax, true);

        // Pass ONLY your scope-trapped target metrics to be processed by your magnified view profile
        DrawEnts(scopeVM, scopePlayers, vents, LPteam, true);

        drawList->PopClipRect();

        // Draw structural scope frame outer rim circle overlay to complete the pipeline pass
        drawList->AddCircle(ImVec2(centerX, centerY), scopeGlassRadius, IM_COL32(0, 0, 0, 255), 64, 3.0f);
    }
}



inline Vector2 WorldToScreen(FVector TargetLocation, VM::FMinimalViewInfo CameraInfo, float ScreenWidth, float ScreenHeight) {
    Vector2 ScreenLocation = Vector2{};

    FVector CameraLocation = CameraInfo.Location;
    FVector CameraRotation = CameraInfo.Rotation;

    VM::D3DMATRIX tempMatrix = VM::Matrix(CameraRotation, FVector(0, 0, 0));
    FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    FVector vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    FVector vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    FVector vDelta = TargetLocation - CameraLocation;
    FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

    // Check if the point is in front of the camera
    if (vTransformed.z <= 1.0f) {
        // Point is behind or on the camera plane, return an invalid or default position
        return Vector2(-1, -1); // Example: Return an invalid screen position
    }

    float FovAngle = CameraInfo.FOV;
    float ScreenCenterX = ScreenWidth / 2.0f;
    float ScreenCenterY = ScreenHeight / 2.0f;

    // Project 3D point to 2D screen space
    ScreenLocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.0f)) / vTransformed.z;
    ScreenLocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.0f)) / vTransformed.z;

    return ScreenLocation;
}

#endif //DANSQD_ESP_H
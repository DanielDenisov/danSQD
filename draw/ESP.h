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
    IColor WHITE = IM_COL32(34, 189, 39, 255);
    IColor WHITE_TRANS = withAlpha(WHITE, TRANS_VAL);

}

inline void ESP(VM::FMinimalViewInfo vm, std::vector<PlayerEnt>& ents, int LPteam) {
    for (PlayerEnt ent : ents) {
        int dist = ent.pos.Dist(vm.Location) / 100; //cm -> m

        //check eligibility of player
        if (ent.health < 0.1f) continue;
        if (ent.pos.Dist(vm.Location) < 10) continue; //skip self. prob not needed cus team check

        ImU32 color = COLOR::RED;
        if (ent.teamID == LPteam) color = COLOR::GREEN_TRANS;

        //Some weird inversion glitch here, but it prob does not matter too much
        // just makes the math look very wrong
        FVector head = ent.pos; head.z += 90.f;
        FVector feet = ent.pos; feet.z -= 90.f;
        Vector2 sfeet = WorldToScreen(head, vm, config::SCREEN_W, config::SCREEN_H);
        Vector2 shead = WorldToScreen(feet, vm, config::SCREEN_W, config::SCREEN_H);

        int gap = getProportionalGap(dist, config::FarthestPlayerDist);
        if (ent.teamID == LPteam) {
            DrawCircleFilled(head.x, head.y - gap - 10, 5, color);
        } else {
            int height = sfeet.y - shead.y;
            int w = height/2;
            DrawBox(shead.x - w/2, shead.y, w, height, color);

            //health stuff
            int lineX = shead.x - w/2 + gap + 4;
            DrawLine(lineX, shead.y, lineX, sfeet.y, COLOR::GREEN_TRANS);
            if (ent.health < 100.f) {
                int YStop = shead.y + (height * (ent.health/100));
                DrawLine(lineX, sfeet.y, lineX, YStop, COLOR::RED_TRANS);
            }
        }


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
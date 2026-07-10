#ifndef DANSQD_ESP_H
#define DANSQD_ESP_H

#include "config.h"
#include "utils/localUtil.h"

inline FVector WorldToScreen(FVector TargetLocation, VM::FMinimalViewInfo CameraInfo, float ScreenWidth, float ScreenHeight) {
    FVector ScreenLocation = FVector(0, 0, 0);

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
        return FVector(-1, -1, 0); // Example: Return an invalid screen position
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
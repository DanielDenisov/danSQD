#pragma once

#include "config.h"

struct FVector {
    double x{}, y{}, z{};

    FVector() = default;
    FVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}


    double Dot(const FVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    double Dist(const FVector& other) const {
        return std::sqrt(std::pow(x - other.x, 2) +
                         std::pow(y - other.y, 2) +
                         std::pow(z - other.z, 2));
    }

    bool operator==(const FVector& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    //Returns new FVector
    FVector operator+(const FVector& v) const
    {
        return FVector(x + v.x, y + v.y, z + v.z);
    }
    //Modifies object
    FVector& operator+=(const FVector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    //Returns new FVector
    FVector operator-(const FVector& v) const
    {
        return FVector(x - v.x, y - v.y, z - v.z);
    }
    //Returns new FVector
    FVector operator*(double number) const {
        return FVector(x * number, y * number, z * number);
    }

    double Magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }
    FVector Normalize() const {
        FVector vector;
        double length = this->Magnitude();

        if (length != 0) {
            vector.x = x / length;
            vector.y = y / length;
            vector.z = z / length;
        }
        else {
            vector.x = vector.y = 0.0f;
            vector.z = 1.0f; // Convention for zero-length vector
        }
        return vector;
    }


    void Print() {
        std::cout << x << " " << y << " " << z << std::endl;
    }
};




struct PlayerStateFlags {
private:
    uint8_t raw; // Holds the 0x2C2 byte containing all 6 bits

public:
    // Standard implicit conversion allows: if (ReadMem<PlayerStateFlags>(state + 0x2C2).bIsABot())
    bool bShouldUpdateReplicatedPing() const { return (raw & (1 << 0)) != 0; }
    bool bIsSpectator()                const { return (raw & (1 << 1)) != 0; }
    bool bOnlySpectator()              const { return (raw & (1 << 2)) != 0; }
    bool bIsABot()                     const { return (raw & (1 << 3)) != 0; }
    bool bIsInactive()                 const { return (raw & (1 << 4)) != 0; }
    bool bFromPreviousLevel()          const { return (raw & (1 << 5)) != 0; }
};


namespace VM {

    struct FMinimalViewInfo {
        FVector Location;
        FVector Rotation;
        float FOV;

        void Print() const {
            printf("========== [ Camera View Info ] ==========\n");
            printf("[+] Location : X: %7.2f | Y: %7.2f | Z: %7.2f\n", Location.x, Location.y, Location.z);
            printf("[+] Rotation : P: %7.2f | Y: %7.2f | R: %7.2f\n", Rotation.x, Rotation.y, Rotation.z);
            printf("[+] FOV      : %.2f\n", FOV);
            printf("==========================================\n");
        }
    };

    struct FCameraCacheEntry {
        float timestamp{};
        char pad_0x0004[12];
        FMinimalViewInfo viewInfo;
    };

    struct D3DMATRIX {
        union {
            struct {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
            float m[4][4];
        };
    };

    inline D3DMATRIX Matrix(FVector rot, FVector origin) {
        float radPitch = (rot.x * M_PI / 180.f);
        float radYaw = (rot.y * M_PI / 180.f);
        float radRoll = (rot.z * M_PI / 180.f);

        float SP = sinf(radPitch), CP = cosf(radPitch);
        float SY = sinf(radYaw), CY = cosf(radYaw);
        float SR = sinf(radRoll), CR = cosf(radRoll);

        D3DMATRIX matrix;
        matrix.m[0][0] = CP * CY;
        matrix.m[0][1] = CP * SY;
        matrix.m[0][2] = SP;
        matrix.m[0][3] = 0.f;

        matrix.m[1][0] = SR * SP * CY - CR * SY;
        matrix.m[1][1] = SR * SP * SY + CR * CY;
        matrix.m[1][2] = -SR * CP;
        matrix.m[1][3] = 0.f;

        matrix.m[2][0] = -(CR * SP * CY + SR * SY);
        matrix.m[2][1] = CY * SR - CR * SP * SY;
        matrix.m[2][2] = CR * CP;
        matrix.m[2][3] = 0.f;

        matrix.m[3][0] = origin.x;
        matrix.m[3][1] = origin.y;
        matrix.m[3][2] = origin.z;
        matrix.m[3][3] = 1.f;

        return matrix;
    }

}
//backup plan if the other W2S does not work
//only use for world to screen.
// namespace VM {
//     struct FVector {
//         double x{}, y{}, z{};
//
//         FVector() = default;
//
//         FVector(Vector3 v3) {
//             x = v3.x; y = v3.y; z = v3.z;
//         }
//
//         Vector3 toV3() {
//             return Vector3{x, y, z};
//         }
//     };
//
//     struct FRotator   { double pitch{}, yaw{}, roll{}; };
//
//     struct FMinimalViewInfo {
//         FVector  location{};
//         FRotator rotation{};
//         float    fov{};
//         float    desiredFov{};
//         float    orthoWidth{};
//         float    orthoNearClip{};
//         float    orthoFarClip{};
//         float    aspectRatio{};
//
//         //more values not not really important
//
//         void Print() const {
//             printf("========== [ Camera View Info ] ==========\n");
//             printf("[+] Location : X: %7.2f | Y: %7.2f | Z: %7.2f\n", location.x, location.y, location.z);
//             printf("[+] Rotation : P: %7.2f | Y: %7.2f | R: %7.2f\n", location.x, location.y, location.z);
//             printf("[+] FOV      : %.2f\n", fov);
//             printf("==========================================\n");
//         }
//     };
//
//     struct FCameraCacheEntry {
//         float timestamp{};
//         char pad_0x0004[12];
//         FMinimalViewInfo viewInfo;
//     };
// }
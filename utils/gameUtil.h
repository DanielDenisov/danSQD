#pragma once

#include "config.h"

struct Vector3 {
    double x, y, z;

    // Constructor
    Vector3(double _x = 0.0, double _y = 0.0, double _z = 0.0)
        : x(_x), y(_y), z(_z) {}

    // Dot product
    double Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Subtraction operator
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    double Dist(const Vector3& other) const {
        return std::sqrt(std::pow(x - other.x, 2) +
                         std::pow(y - other.y, 2) +
                         std::pow(z - other.z, 2));
    }

    bool operator==(const Vector3& other) const {
        return (x == other.x && y == other.y && z == other.z);
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

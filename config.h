#ifndef DANSQD_CONFIG_H
#define DANSQD_CONFIG_H

namespace config {
    const inline int SCREEN_H = 1405;
    const inline int SCREEN_W = 2560;

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

}

namespace vtable {

}

#endif //DANSQD_CONFIG_H
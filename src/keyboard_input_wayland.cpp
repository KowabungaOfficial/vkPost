#include <xkbcommon/xkbcommon.h>
uint32_t convertToKeySymWayland(std::string key) {
    return xkb_keysym_from_name(key.c_str(), XKB_KEYSYM_NO_FLAGS);
}

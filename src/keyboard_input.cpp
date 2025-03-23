#include "keyboard_input.hpp"
#include "logger.hpp"

#ifndef VKPOST_X11
#define VKPOST_X11 1
#endif

#ifndef VKPOST_WAYLAND
#define VKPOST_WAYLAND 1
#endif

#if VKPOST_X11
#include "keyboard_input_x11.hpp"
#endif

#if VKPOST_WAYLAND
#include "keyboard_input_wayland.hpp"
#endif

namespace vkPost
{
    // Helper function to check if Wayland is active
    bool isWaylandActive()
    {
        const char* sessionType = getenv("XDG_SESSION_TYPE");
        return sessionType && strcmp(sessionType, "wayland") == 0;
    }

    uint32_t convertToKeySym(std::string key)
    {
#if VKPOST_WAYLAND
        if (isWaylandActive()) {
            return convertToKeySymWayland(key);
        }
#endif
#if VKPOST_X11
        return convertToKeySymX11(key);
#endif
        return 0u;
    }

    bool isKeyPressed(uint32_t ks)
    {
#if VKPOST_WAYLAND
        if (isWaylandActive()) {
            return isKeyPressedWayland(ks);
        }
#endif
#if VKPOST_X11
        return isKeyPressedX11(ks);
#endif
        return false;
    }
} // namespace vkPost

#include "keyboard_input.hpp"
#include "display_server.hpp"
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

namespace vkPost {
    uint32_t convertToKeySym(DisplayServer ds, std::string key) {
#if VKPOST_X11
        if (ds == DisplayServer::X11) {
            return convertToKeySymX11(key);
        }
#endif
#if VKPOST_WAYLAND
        if (ds == DisplayServer::WAYLAND) {
            return convertToKeySymWayland(key);
        }
#endif
        Logger::err("No display server support for key conversion");
        return 0u;
    }

    bool isKeyPressed(DisplayServer ds, uint32_t ks) {
#if VKPOST_X11
        if (ds == DisplayServer::X11) {
            return isKeyPressedX11(ks);
        }
#endif
#if VKPOST_WAYLAND
        if (ds == DisplayServer::WAYLAND) {
            return isKeyPressedWayland(ks);
        }
#endif
        return false;
    }
}

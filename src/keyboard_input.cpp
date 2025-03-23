#include "keyboard_input.hpp"
#include "logger.hpp"
#include "config.hpp"

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
    static InputMethod currentInputMethod = InputMethod::None;

    // Check if Wayland is available
    static bool isWaylandAvailable()
    {
#if VKPOST_WAYLAND
        const char* sessionType = getenv("XDG_SESSION_TYPE");
        if (sessionType && strcmp(sessionType, "wayland") == 0) {
            struct wl_display* display = wl_display_connect(nullptr);
            if (display) {
                wl_display_disconnect(display);
                return true;
            }
        }
        return false;
#else
        return false;
#endif
    }

    // Check if X11 is available
    static bool isX11Available()
    {
        const char* displayVar = getenv("DISPLAY");
        if (displayVar && strcmp(displayVar, "") != 0) {
            Display* display = XOpenDisplay(displayVar);
            if (display) {
                XCloseDisplay(display);
                return true;
            }
        }
        return false;
    }

    // Set the input method based on config
    void setInputMethodBasedOnConfig(const Config& config)
    {
        std::string mode = config.getOption<std::string>("keyboardInputMode", "auto");

        if (mode == "auto") {
            if (isWaylandAvailable()) {
                currentInputMethod = InputMethod::Wayland;
            } else if (isX11Available()) {
                currentInputMethod = InputMethod::X11;
            } else {
                currentInputMethod = InputMethod::None;
                Logger::warn("No keyboard input method available, disabling input");
            }
        } else if (mode == "x11") {
            if (isX11Available()) {
                currentInputMethod = InputMethod::X11;
            } else {
                currentInputMethod = InputMethod::None;
                Logger::warn("X11 is not available, disabling keyboard input");
            }
        } else if (mode == "wayland") {
            if (isWaylandAvailable()) {
                currentInputMethod = InputMethod::Wayland;
            } else {
                currentInputMethod = InputMethod::None;
                Logger::warn("Wayland is not available, disabling keyboard input");
            }
        } else if (mode == "none") {
            currentInputMethod = InputMethod::None;
        } else {
            currentInputMethod = InputMethod::None;
            Logger::err("Invalid keyboardInputMode: " + mode + ", disabling keyboard input");
        }
    }

    // Initialize the selected input method
    void initializeKeyboardInput()
    {
        if (currentInputMethod == InputMethod::Wayland) {
#if VKPOST_WAYLAND
            init_wayland_data();
#endif
        }
        // X11 initialization is handled lazily in isKeyPressedX11
    }

    uint32_t convertToKeySym(std::string key)
    {
        if (currentInputMethod == InputMethod::X11) {
            return convertToKeySymX11(key);
        } else if (currentInputMethod == InputMethod::Wayland) {
#if VKPOST_WAYLAND
            return convertToKeySymWayland(key);
#else
            return 0u;
#endif
        } else {
            return 0u; // Input disabled
        }
    }

    bool isKeyPressed(uint32_t ks)
    {
        if (currentInputMethod == InputMethod::X11) {
            return isKeyPressedX11(ks);
        } else if (currentInputMethod == InputMethod::Wayland) {
#if VKPOST_WAYLAND
            return isKeyPressedWayland(ks);
#else
            return false;
#endif
        } else {
            return false; // Input disabled
        }
    }
} // namespace vkPost

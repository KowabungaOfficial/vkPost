#pragma once

#include <cstdint>
#include <string>
#include "config.hpp"

namespace vkPost
{
    enum class InputMethod {
        X11,
        Wayland,
        NoInput
    };

    uint32_t convertToKeySym(std::string key);
    bool     isKeyPressed(uint32_t ks);
#if VKPOST_WAYLAND
    uint32_t convertToKeySymWayland(std::string key);
    bool     isKeyPressedWayland(uint32_t ks);
    void     init_wayland_data();
#endif

    void setInputMethodBasedOnConfig(const Config& config);
    void initializeKeyboardInput();
} // namespace vkPost

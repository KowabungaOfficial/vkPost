#pragma once

#include <cstdint>
#include <string>

namespace vkPost
{
    uint32_t convertToKeySymWayland(std::string key);
    bool     isKeyPressedWayland(uint32_t ks);
    void     init_wayland_data();
} // namespace vkPost

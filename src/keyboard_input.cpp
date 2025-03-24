#include "keyboard_input.hpp"

#include "logger.hpp"

// TODO build without X11
#ifndef VKPOST_X11
#define VKPOST_X11 1
#endif

#if VKPOST_X11
#include "keyboard_input_x11.hpp"
#endif

namespace vkPost
{
    uint32_t convertToKeySym(std::string key)
    {
#if VKPOST_X11
        return convertToKeySymX11(key);
#endif
        return 0u;
    }

    bool isKeyPressed(uint32_t ks)
    {
#if VKPOST_X11
        return isKeyPressedX11(ks);
#endif
        return false;
    }
} // namespace vkPost
